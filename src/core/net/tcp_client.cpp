/**
 * Copyright 2025 Viper authors.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
**/

#include "core/net/tcp_client.h"
#include "core/error/error.h"
#include "core/log/log.h"
#include "core/net/message.h"
#include "core/net/tcp_connection.h"
#include "core/net/tcp_handler.h"

#include <event2/bufferevent.h>
#include <event2/event.h>
#include <event2/util.h>

#include <cmath>
#include <memory.h>
#include <string>

namespace viper {
namespace net {

TCPClient::TCPClient()
{
}

TCPClient::~TCPClient()
{
}

void TCPClient::ReadCallback(bufferevent* bev, void* ctx)
{
    auto conn = static_cast<TCPConnection*>(ctx);

    MessagePtr msg     = std::make_shared<Message>();
    auto       errcode = conn->Read(msg);

    if (errcode == error::ErrorCode::SYSTEM_TRY_AGAIN)
    {
        LOG_DEBUG("no more data to read, try again, connection:{}", conn->ID());
        return;
    }

    if (errcode == error::ErrorCode::NET_INVALID_MAGIC)
    {
        LOG_WARN("invalid connection:{}", conn->ID());
        conn->UpdateState(ConnectionState::INVALID);
        return;
    }

    if (!error::IsSuccess(errcode))
    {
        LOG_ERROR("failed to read data from connection:{}, errcode:{}", conn->GetRemoteAddress(), errcode.value());
    }

    LOG_DEBUG("readed a message. connection:{}", conn->ID());

    auto handler    = static_cast<TCPClient*>(conn->GetHandler());
    auto sharedConn = conn->shared_from_this();

    if (!handler->ProcessCoreMessage(sharedConn, msg))
    {
        handler->_functor->HandleData(sharedConn, msg);
    }
}

void TCPClient::EventCallback(bufferevent* bev, short events, void* ctx)
{
    auto       conn   = static_cast<TCPConnection*>(ctx);
    TCPClient* client = (TCPClient*)conn->GetHandler();

    if (events & BEV_EVENT_CONNECTED)
    {
        conn->UpdateState(ConnectionState::CONNECTED);
        client->_functor->OnConnection(conn->shared_from_this());
        return;
    }

    if (events & BEV_EVENT_EOF)
    {
        conn->UpdateState(ConnectionState::DISCONNECTED);
        auto sharedConn = conn->shared_from_this();
        client->_functor->OnDisconnection(sharedConn);
        client->_connection.reset();
        return;
    }

    if (events & BEV_EVENT_ERROR)
    {
    }

    if (events & BEV_EVENT_TIMEOUT)
    {
        LOG_WARN("connection timeout. connection:{}", conn->ID());
    }
}

void TCPClient::CheckConnectionState(evutil_socket_t fd, short events, void* ctx)
{
    auto client = static_cast<TCPClient*>(ctx);

    LOG_DEBUG("tcp client check the connection state, remote server: {}:{}", client->_remoteIP, client->_remotePort);

    if (client->_connection == nullptr || client->_connection->State() != ConnectionState::CONNECTED)
    {
        auto errcode = client->Reconnect();
        if (!viper::error::IsSuccess(errcode))
        {
            LOG_WARN("failed to reconnect. remote server:{}:{}", client->_remoteIP, client->_remotePort);
        }
    }

    // set the next timer
    evtimer_add(client->_checkConnectionStateEvent, &client->_checkConnectionTimeoutSeconds);
}

void TCPClient::ConnectionKeepalive(evutil_socket_t fd, short events, void* ctx)
{
    auto client = static_cast<TCPClient*>(ctx);

    LOG_DEBUG("tcp client connection keepalive, remote server: {}:{}", client->_remoteIP, client->_remotePort);

    if (client->_connection == nullptr)
    {
        // set the next timer
        evtimer_add(client->_connectionKeepaliveEvent, &client->_connectionKeepaliveTimeoutSeconds);
        return;
    }

    static std::string data(VIPER_NET_MESSAGE_KEEPALIVE_PING);

    viper::net::Header header;
    header._dataSize = data.size();
    header._msgType  = (uint32_t)VIPER_NET_MESSAGE_PROTOCOL_KEEPALIVE_PING;

    viper::net::Hton(header);
    viper::net::Message msg(header, data.data(), data.size());

    auto errcode = client->_connection->Send(msg);

    if (!viper::error::IsSuccess(errcode))
    {
        LOG_WARN("failed to send keepalive ping. connection: {}", client->_connection->ID());
    }

    // set the next timer
    evtimer_add(client->_connectionKeepaliveEvent, &client->_connectionKeepaliveTimeoutSeconds);
}

void TCPClient::SetTimeout(int timeoutSec)
{
    if (timeoutSec < VIPER_NET_TCP_CONNECTION_TIMEOUT_SECOND_DFT)
    {
        _checkConnectionTimeoutSeconds.tv_sec     = VIPER_NET_TCP_CONNECTION_TIMEOUT_SECOND_DFT;
        _connectionKeepaliveTimeoutSeconds.tv_sec = VIPER_NET_TCP_CONNECTION_KEEPALIVE_TIMEOUT_SECOND_DFT;
        return;
    }

    _checkConnectionTimeoutSeconds.tv_sec     = timeoutSec;
    _connectionKeepaliveTimeoutSeconds.tv_sec = std::floor(timeoutSec / 2);
}

void TCPClient::SetCallback(TCPHandlerCallbackFunctor functor)
{
    _functor = functor;
}

std::error_code TCPClient::Connect(const std::string& ip, uint16_t port)
{
    _base = event_base_new();
    if (!_base)
    {
        LOG_ERROR("failed to create evnet base:{}:{}", ip, port);
        return error::ErrorCode::SYSTEM_LIB_EXCEPTION;
    }

    _remoteIP   = ip;
    _remotePort = port;

    auto errcode = Reconnect();
    if (!viper::error::IsSuccess(errcode))
    {
        return errcode;
    }

    _asyncRun = std::async(std::launch::async, &TCPClient::Run, this);
    return error::ErrorCode::SUCCESS;
}

void TCPClient::Close()
{
    if (!_base)
    {
        return;
    }

    event_base_loopbreak(_base);
    bufferevent_free(_bev);
    event_base_free(_base);

    _base = nullptr;
    _bev  = nullptr;

    _asyncRun.wait();
}

std::error_code TCPClient::Send(const Message& msg)
{
    if (_connection == nullptr)
    {
        return error::ErrorCode::NET_DISCONNECTED;
    }

    if (_connection->State() != ConnectionState::CONNECTED)
    {
        return viper::error::ErrorCode::NET_DISCONNECTED;
    }

    return _connection->Send(msg);
}

void TCPClient::Run()
{
    // set connection state check timer
    _checkConnectionStateEvent = evtimer_new(_base, &TCPClient::CheckConnectionState, this);
    evtimer_add(_checkConnectionStateEvent, &_checkConnectionTimeoutSeconds);

    // set connection keepalive timer
    _connectionKeepaliveEvent = evtimer_new(_base, &TCPClient::ConnectionKeepalive, this);
    evtimer_add(_connectionKeepaliveEvent, &_connectionKeepaliveTimeoutSeconds);

    // start the event loop
    int exitedCode = 0;
    do {
        exitedCode = event_base_loop(_base, EVLOOP_NO_EXIT_ON_EMPTY);
    } while (exitedCode != -1);

    LOG_WARN("tcp client run exited. exited code:{}", exitedCode);
    bufferevent_free(_bev);
}

std::error_code TCPClient::Reconnect()
{
    evutil_addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family   = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    auto             remotePort = std::to_string(_remotePort);
    evutil_addrinfo* servinfo;

    if (getaddrinfo(_remoteIP.c_str(), remotePort.c_str(), &hints, &servinfo) != 0)
    {
        LOG_WARN("failed to get addr info. remote server: {}:{}", _remoteIP, remotePort);
        event_base_free(_base);
        _base = nullptr;
        return error::ErrorCode::NET_DISCONNECTED;
    }

    evutil_addrinfo* p = nullptr;
    for (p = servinfo; p != nullptr; p = p->ai_next)
    {
        _bev = bufferevent_socket_new(_base, -1, BEV_OPT_CLOSE_ON_FREE);
        if (bufferevent_socket_connect(_bev, p->ai_addr, p->ai_addrlen) < 0)
        {
            bufferevent_free(_bev);
            _bev = nullptr;
            continue;
        }

        auto conn = std::make_shared<TCPConnection>(-1, p->ai_addr, p->ai_addrlen);
        conn->UpdateState(ConnectionState::CONNECTING);
        conn->BindHandler(_bev, this);
        bufferevent_setcb(_bev, &TCPClient::ReadCallback, nullptr, &TCPClient::EventCallback, conn.get());
        bufferevent_enable(_bev, EV_READ | EV_WRITE);
        _connection = conn;
        break;
    }

    freeaddrinfo(servinfo);
    return error::ErrorCode::SUCCESS;
}

bool TCPClient::ProcessCoreMessage(TCPConnectionPtr conn, MessagePtr msg)
{
    const auto& header = msg->GetHeader();

    if (header._msgType > VIPER_NET_MESSAGE_PROTOCOL_BASE)
    {
        return false;
    }

    if (header._msgType == VIPER_NET_MESSAGE_PROTOCOL_KEEPALIVE_PONG)
    {
        LOG_DEBUG("received keepalive pong. remote server: {}", conn->GetRemoteAddress());
        return true;
    }

    return true;
}

} // namespace net
} // namespace viper

