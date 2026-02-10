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

#include "core/net/tcp_connection.h"
#include "core/assist/string.h"
#include "core/assist/time.h"
#include "core/error/error.h"
#include "core/log/log.h"
#include "core/net/message.h"

#include <event2/buffer.h>
#include <event2/bufferevent.h>
#include <event2/event.h>

namespace viper {
namespace net {

TCPConnection::TCPConnection(evutil_socket_t fd, sockaddr* address, int socklen)
{
    _fd                      = fd;
    char host[NI_MAXHOST]    = {0};
    char service[NI_MAXSERV] = {0};
    int  flags               = NI_NUMERICHOST | NI_NUMERICSERV;
    if (getnameinfo(address, sizeof(sockaddr_in), host, NI_MAXHOST, service, NI_MAXSERV, flags) != 0)
    {
        throw std::runtime_error("getnameinfo() failed");
    }

    _remoteIP   = host;
    _remotePort = std::atoi(service);
    BuildID();
}

TCPConnection::~TCPConnection()
{
    LOG_DEBUG("connection is disconnected. {}", _id);

    if (_bev)
    {
        bufferevent_disable(_bev, EV_WRITE | EV_READ);
        bufferevent_free(_bev);
        _bev = nullptr;
    }
}

const std::string& TCPConnection::ID()
{
    return _id;
}

void TCPConnection::UpdateState(ConnectionState state)
{
    _state.store(state);
}

bool TCPConnection::IsTimeout(int timeoutSec)
{
    if (0 == _lastReadTimestamp)
    {
        _lastReadTimestamp = assist::TimestampTickCountSecond();
        return false;
    }

    // A timeout is only recognized if it exceeds the timeout period five times.
    auto duration = assist::TimestampTickCountSecond() - _lastReadTimestamp;
    auto timeout  = timeoutSec * 5;
    LOG_DEBUG("check timeout. duration: {}, timeout seconds: {}, connection: {}", duration, timeout, ID());
    return duration >= timeout;
}

ConnectionState TCPConnection::State()
{
    return _state;
}

void TCPConnection::BindHandler(bufferevent* bev, void* handler)
{
    _bev     = bev;
    _handler = handler;
}

void* TCPConnection::GetHandler()
{
    return _handler;
}

std::string TCPConnection::GetRemoteAddress()
{
    return assist::FormatString("%s:%d", _remoteIP.c_str(), _remotePort);
}

std::error_code TCPConnection::Read(MessagePtr msg)
{
    evbuffer* buffer = bufferevent_get_input(_bev);
    if (Message::MESSAGE_HEADER_SIZE > evbuffer_get_length(buffer))
    {
        LOG_DEBUG("evbuffer length less than header size");
        return error::ErrorCode::SYSTEM_TRY_AGAIN;
    }

    Header header;
    int    length = evbuffer_copyout(buffer, (void*)&header, Message::MESSAGE_HEADER_SIZE);
    if (Message::MESSAGE_HEADER_SIZE > length)
    {
        LOG_DEBUG("evbuffer copied size less than header size");
        return error::ErrorCode::SYSTEM_TRY_AGAIN;
    }

    Ntoh(header);

    uint32_t totalSize = header._dataSize + Message::MESSAGE_HEADER_SIZE;
    if (totalSize > Message::MAX_MESSAGE_SIZE)
    {
        LOG_WARN("total size {} more than max message size {}.", totalSize, (uint64_t)Message::MAX_MESSAGE_SIZE);
        return error::ErrorCode::NET_MESSAGE_TOO_LARGE;
    }

    if (header._magic != VIPER_NET_MESSAGE_MAGIC)
    {
        LOG_WARN("invalid connection: {}, magic: 0x{:04X}", ID(), header._magic);
        return error::ErrorCode::NET_INVALID_MAGIC;
    }

    _lastReadTimestamp = assist::TimestampTickCountSecond();
    UpdateState(ConnectionState::CONNECTED);

    char* msgBuffer = (char*)evbuffer_pullup(buffer, totalSize);
    if (!msgBuffer)
    {
        LOG_DEBUG("no more data to read, connection: {}", ID());
        return error::ErrorCode::SYSTEM_TRY_AGAIN;
    }

    LOG_DEBUG("received, body size:{}, total size:{}", header._dataSize, totalSize);
    msg->Reset(header, msgBuffer + Message::MESSAGE_HEADER_SIZE, header._dataSize);
    evbuffer_drain(buffer, totalSize);

    return error::ErrorCode::SUCCESS;
}

std::error_code TCPConnection::Send(const Message& msg)
{
    LOG_DEBUG("send data. size:{}, remote address:{}", msg.GetDataSize(), GetRemoteAddress());
    int errcode = bufferevent_write(_bev, msg.GetData(), msg.GetDataSize());
    if (errcode)
    {
        return error::ErrorCode::NET_SEND_FAILED;
    }
    return error::ErrorCode::SUCCESS;
}

std::error_code TCPConnection::Send(const MessagePtr msg)
{
    return Send(*msg);
}

void TCPConnection::BuildID()
{
    _id = assist::FormatString("%s-%d", _remoteIP.c_str(), _remotePort);
    assist::Trim(_id, ":-.");
}

} // namespace net
} // namespace viper

