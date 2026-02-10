#include "core/net/tcp_handler.h"
#include "core/error/error.h"
#include "core/log/log.h"
#include "core/net/message.h"
#include "core/net/tcp_connection.h"

#include <bits/types/struct_timeval.h>
#include <event2/bufferevent.h>
#include <event2/event.h>

#include <future>
#include <string>

namespace viper {
namespace net {

TCPHandler::TCPHandler()
{
}

TCPHandler::~TCPHandler()
{
}

void TCPHandler::CheckConnectionState(evutil_socket_t fd, short events, void* ctx)
{
    auto server = static_cast<TCPHandler*>(ctx);

    LOG_DEBUG("tcp server handler check the connection state, connection count: {}", server->_connections.Count());

    server->_connections.Delete([server](std::string key, TCPConnectionPtr conn) -> bool {
        if (conn->State() != ConnectionState::CONNECTED)
        {
            // delete this connection
            return true;
        }

        if (conn->IsTimeout(server->_timeoutSeconds.tv_sec))
        {
            LOG_DEBUG("timeout. connection: {}", conn->ID());
            conn->UpdateState(ConnectionState::TIMEOUT);
            return false;
        }

        return false;
    });

    evtimer_add(server->_checkConnectionStateEvent, &server->_timeoutSeconds);
}

void TCPHandler::SetTimeout(int timeoutSec)
{
    _timeoutSeconds.tv_sec = timeoutSec;
}

void TCPHandler::ReadCallback(bufferevent* bev, void* ctx)
{
    TCPConnection* conn = static_cast<TCPConnection*>(ctx);

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

    auto handler    = static_cast<TCPHandler*>(conn->GetHandler());
    auto sharedConn = conn->shared_from_this();

    if (!handler->ProcessCoreMessage(sharedConn, msg))
    {
        handler->_functor->HandleData(sharedConn, msg);
    }
}
void TCPHandler::EventCallback(bufferevent* bev, short events, void* ctx)
{
    TCPConnection* conn = static_cast<TCPConnection*>(ctx);
    if (events & BEV_EVENT_EOF)
    {
        conn->UpdateState(ConnectionState::CONNECTED);
        auto handler = static_cast<TCPHandler*>(conn->GetHandler());
        handler->_functor->OnDisconnection(conn->shared_from_this());
        handler->_connections.Delete(conn->ID());
        bufferevent_free(bev);
        return;
    }

    if (events & BEV_EVENT_ERROR)
    {
        LOG_ERROR("make some exception from bufferevent");
    }
}

void TCPHandler::SetCallback(TCPHandlerCallbackFunctor functor)
{
    _functor = functor;
}

void TCPHandler::BindConnection(evutil_socket_t fd, sockaddr* address, int socklen)
{
    auto conn = std::make_shared<TCPConnection>(fd, address, socklen);
    auto bev  = bufferevent_socket_new(_base, fd, BEV_OPT_CLOSE_ON_FREE);

    conn->UpdateState(ConnectionState::CONNECTED);
    _functor->OnConnection(conn);

    bufferevent_setcb(bev, ReadCallback, nullptr, EventCallback, conn.get());
    bufferevent_setwatermark(bev, EV_READ, Message::MESSAGE_HEADER_SIZE, 10 * 1024 * 1024);
    bufferevent_enable(bev, EV_READ | EV_WRITE);

    timeval timeout;
    timeout.tv_sec  = _timeoutSeconds.tv_sec;
    timeout.tv_usec = 0;
    bufferevent_set_timeouts(bev, &timeout, &timeout);

    conn->BindHandler(bev, this);
    _connections.Push(conn->ID(), conn);
}

std::error_code TCPHandler::Start()
{
    _base = event_base_new();
    if (!_base)
    {
        LOG_ERROR("failed to create event base");
        return error::ErrorCode::SYSTEM_LIB_EXCEPTION;
    }

    _asyncRun = std::async(std::launch::async, &TCPHandler::Run, this);

    return error::ErrorCode::SUCCESS;
}

std::error_code TCPHandler::Stop()
{
    if (!_base)
    {
        return error::ErrorCode::SUCCESS;
    }

    event_base_loopbreak(_base);
    event_free(_checkConnectionStateEvent);
    event_base_free(_base);

    _base                      = nullptr;
    _checkConnectionStateEvent = nullptr;

    return error::ErrorCode::SUCCESS;
}

void TCPHandler::Run()
{
    // set timer callback
    _checkConnectionStateEvent = evtimer_new(_base, &TCPHandler::CheckConnectionState, this);
    evtimer_add(_checkConnectionStateEvent, &_timeoutSeconds);

    // start event loop
    int exitedCode = 0;
    do {
        exitedCode = event_base_loop(_base, EVLOOP_NO_EXIT_ON_EMPTY);
    } while (exitedCode != -1);

    LOG_WARN("tcp handler run exited. exited code:{}", exitedCode);
}

bool TCPHandler::ProcessCoreMessage(TCPConnectionPtr conn, MessagePtr msg)
{
    const auto& header = msg->GetHeader();

    if (header._msgType > VIPER_NET_MESSAGE_PROTOCOL_BASE)
    {
        LOG_DEBUG("tcp handler, is not core message protocol");
        return false;
    }

    if (header._msgType == VIPER_NET_MESSAGE_PROTOCOL_KEEPALIVE_PING)
    {
        LOG_DEBUG("received keepalive ping. client: {}", conn->GetRemoteAddress());

        static std::string data(VIPER_NET_MESSAGE_KEEPALIVE_PONG);
        viper::net::Header   header;
        header._dataSize = data.size();
        header._msgType  = (uint32_t)VIPER_NET_MESSAGE_PROTOCOL_KEEPALIVE_PONG;

        viper::net::Hton(header);

        viper::net::Message msg(header, data.data(), data.size());

        auto errcode = conn->Send(msg);

        if (!viper::error::IsSuccess(errcode))
        {
            LOG_WARN("failed to respond keepalive pong. connection: {}", conn->ID());
        }

        return true;
    }

    return true;
}

} // namespace net
} // namespace viper

