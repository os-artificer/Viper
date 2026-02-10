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

#ifndef _VIPER_CORE_NET_TCP_CLIENT_H_
#define _VIPER_CORE_NET_TCP_CLIENT_H_

#include "core/net/message.h"
#include "core/net/tcp_connection.h"
#include "core/net/tcp_handler.h"

#include <event2/bufferevent.h>
#include <event2/event.h>

#include <cstdint>
#include <future>
#include <memory>
#include <system_error>

namespace viper {
namespace net {

class TCPClient final
{
public:
    TCPClient();
    ~TCPClient();

public:
    static void ReadCallback(bufferevent* bev, void* ctx);
    static void EventCallback(bufferevent* bev, short events, void* ctx);
    static void CheckConnectionState(evutil_socket_t fd, short events, void* ctx);
    static void ConnectionKeepalive(evutil_socket_t fd, short events, void* ctx);

public:
    void            SetTimeout(int timeoutSec);
    void            SetCallback(TCPHandlerCallbackFunctor functor);
    std::error_code Connect(const std::string& ip, uint16_t port);
    void            Close();
    std::error_code Send(const Message& msg);

private:
    void            Run();
    std::error_code Reconnect();
    bool            ProcessCoreMessage(TCPConnectionPtr conn, MessagePtr msg);

private:
    timeval                   _checkConnectionTimeoutSeconds     = {VIPER_NET_TCP_CONNECTION_TIMEOUT_SECOND_DFT, 0};
    timeval                   _connectionKeepaliveTimeoutSeconds = {VIPER_NET_TCP_CONNECTION_KEEPALIVE_TIMEOUT_SECOND_DFT, 0};
    TCPHandlerCallbackFunctor _functor                           = nullptr;
    std::string               _remoteIP;
    uint16_t                  _remotePort = 0;
    std::future<void>         _asyncRun;
    TCPConnectionPtr          _connection                = nullptr;
    event*                    _checkConnectionStateEvent = nullptr;
    event*                    _connectionKeepaliveEvent  = nullptr;
    event_base*               _base                      = nullptr;
    bufferevent*              _bev                       = nullptr;
};

using TCPClientPtr = std::shared_ptr<TCPClient>;

} // namespace net
} // namespace viper
#endif

