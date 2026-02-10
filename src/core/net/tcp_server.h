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

#ifndef _VIPER_CORE_NET_TCP_SERVER_H_
#define _VIPER_CORE_NET_TCP_SERVER_H_

#include "core/net/tcp_connection.h"
#include "core/net/tcp_handler.h"

#include <event2/listener.h>

#include <atomic>
#include <cstdint>
#include <event2/util.h>
#include <memory>
#include <vector>

namespace viper {
namespace net {

class TCPServer final
{
public:
    TCPServer(const std::string& listenAddress, uint16_t port, int threadCount = 3);
    ~TCPServer();

public:
    static void AcceptCallback(struct evconnlistener* listener, evutil_socket_t fd,
                               sockaddr* address, int socklen, void* ctx);

public:
    void            SetTimeout(int timeoutSec);
    void            SetCallback(TCPHandlerCallbackFunctor functor);
    std::error_code Run();
    std::error_code Close();

private:
    int         _timeoutSec  = VIPER_NET_TCP_CONNECTION_TIMEOUT_SECOND_DFT;
    int         _threadCount = 0;
    std::string _listenAddress;
    uint16_t    _listenPort = 0;

    TCPHandlerCallbackFunctor _functor  = nullptr;
    event_base*               _base     = nullptr;
    evconnlistener*           _listener = nullptr;

    std::atomic_uint64_t       _handlerIndex = 0;
    std::vector<TCPHandlerPtr> _handlers;
};

using TCPServerPtr = std::shared_ptr<TCPServer>;

} // namespace net
} // namespace viper

#endif
