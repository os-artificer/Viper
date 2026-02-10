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

#ifndef _VIPER_CORE_NET_TCP_CONNECTION_H_
#define _VIPER_CORE_NET_TCP_CONNECTION_H_

#include "core/net/message.h"

#include <event2/buffer.h>
#include <event2/bufferevent.h>
#include <event2/event.h>
#include <event2/util.h>

#include <atomic>
#include <cstdint>
#include <memory>
#include <string>
#include <system_error>

namespace viper {
namespace net {

// clang-format off

#define VIPER_NET_TCP_CONNECTION_TIMEOUT_SECOND_DFT           6
#define VIPER_NET_TCP_CONNECTION_KEEPALIVE_TIMEOUT_SECOND_DFT 3

// clang-format on

enum class ConnectionState : int
{
    CONNECTING,
    CONNECTED,
    DISCONNECTED,
    TIMEOUT,
    INVALID,
    UNKNOWN,
};

class TCPConnection final : public std::enable_shared_from_this<TCPConnection>
{
public:
    TCPConnection(evutil_socket_t fd, sockaddr* address, int socklen);
    ~TCPConnection();

public:
    const std::string& ID();
    void               UpdateState(ConnectionState state);
    bool               IsTimeout(int timeoutSec);
    ConnectionState    State();
    void               BindHandler(bufferevent* bev, void* handler);
    void*              GetHandler();
    std::string        GetRemoteAddress();
    std::error_code    Read(MessagePtr msg);
    std::error_code    Send(const Message& msg);
    std::error_code    Send(const MessagePtr msg);

private:
    void BuildID();

private:
    std::atomic<ConnectionState> _state             = ConnectionState::UNKNOWN;
    uint64_t                     _lastReadTimestamp = 0;

    std::string     _id;
    evutil_socket_t _fd = EVUTIL_INVALID_SOCKET;

    std::string _remoteIP;
    uint16_t    _remotePort = 0;

    void*        _handler = nullptr;
    bufferevent* _bev     = nullptr;
};

using TCPConnectionPtr = std::shared_ptr<TCPConnection>;

} // namespace net
} // namespace viper

#endif

