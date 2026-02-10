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

#ifndef _VIPER_CORE_NET_TCP_HANDLER_H_
#define _VIPER_CORE_NET_TCP_HANDLER_H_

#include "core/container/safe_map.h"
#include "core/net/message.h"
#include "core/net/tcp_connection.h"

#include <event2/bufferevent.h>

#include <future>
#include <memory>

namespace viper {
namespace net {

class TCPHandlerCallback
{
public:
    TCPHandlerCallback()          = default;
    virtual ~TCPHandlerCallback() = default;

public:
    virtual void OnConnection(TCPConnectionPtr conn)                     = 0;
    virtual void OnDisconnection(TCPConnectionPtr conn)                  = 0;
    virtual void HandleData(TCPConnectionPtr conn, const MessagePtr msg) = 0;
};

using TCPHandlerCallbackFunctor = std::shared_ptr<TCPHandlerCallback>;

class TCPHandler final
{
public:
    TCPHandler();
    ~TCPHandler();

public:
    static void CheckConnectionState(evutil_socket_t fd, short events, void* ctx);

public:
    static void ReadCallback(bufferevent* bev, void* ctx);
    static void EventCallback(bufferevent* bev, short events, void* ctx);

public:
    void            SetTimeout(int timeoutSec);
    void            SetCallback(TCPHandlerCallbackFunctor functor);
    void            BindConnection(evutil_socket_t fd, sockaddr* address, int socklen);
    std::error_code Start();
    std::error_code Stop();

private:
    void Run();
    bool ProcessCoreMessage(TCPConnectionPtr conn, MessagePtr msg);

private:
    timeval                   _timeoutSeconds            = {VIPER_NET_TCP_CONNECTION_TIMEOUT_SECOND_DFT, 0};
    TCPHandlerCallbackFunctor _functor                   = nullptr;
    event*                    _checkConnectionStateEvent = nullptr;
    event_base*               _base                      = nullptr;
    std::future<void>         _asyncRun;

    container::SafeMap<std::string, TCPConnectionPtr> _connections;
};

using TCPHandlerPtr = std::shared_ptr<TCPHandler>;

} // namespace net
} // namespace viper
#endif

