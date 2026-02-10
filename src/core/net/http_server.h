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

#ifndef _VIPER_NET_HTTP_SERVER_H_
#define _VIPER_NET_HTTP_SERVER_H_

#include "core/net/http_context.h"

#include <event2/buffer.h>
#include <event2/event.h>
#include <event2/http.h>
#include <event2/http_struct.h>

#include <cstdint>
#include <memory>
#include <system_error>

namespace viper {
namespace net {

class HTTPServer final
{
public:
    HTTPServer();
    ~HTTPServer();

public:
    std::error_code RegisterHandler(HTTPMethod method, const HTTPURI& uri,
                                    HTTPMethodHandler handler);
    void            Run(const std::string& listenIP, uint16_t listenPort);
    void            Close();

private:
    static void RequestHandler(evhttp_request* req, void* arg);

private:
    void UnsupportedRequestHandler(evhttp_request* req);
    void GetRequestHandler(evhttp_request* req);
    void PostRequestHandler(evhttp_request* req);
    void DeleteRequestHandler(evhttp_request* req);
    void PutRequestHandler(evhttp_request* req);

private:
    std::map<HTTPURI, RegisteredHandlerPtr> _getURIHandlers;
    std::map<HTTPURI, RegisteredHandlerPtr> _postURIHandlers;
    std::map<HTTPURI, RegisteredHandlerPtr> _deleteURIHandlers;
    std::map<HTTPURI, RegisteredHandlerPtr> _putURIHandlers;
    std::string                             _listenIP;
    uint16_t                                _listenPort = 0;
    event_base*                             _base       = nullptr;
    evhttp*                                 _httpServer = nullptr;
};

using HTTPServerPtr = std::shared_ptr<HTTPServer>;

} // namespace net
} // namespace viper

#endif

