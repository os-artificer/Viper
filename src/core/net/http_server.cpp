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

#include "core/net/http_server.h"
#include "core/assist/string.h"
#include "core/error/error.h"
#include "core/log/log.h"
#include "core/net/http_context.h"

#include <event2/buffer.h>
#include <event2/event.h>
#include <event2/http.h>
#include <event2/http_struct.h>

#include <new>

namespace viper {
namespace net {

HTTPServer::HTTPServer() {}
HTTPServer::~HTTPServer() {}

std::error_code HTTPServer::RegisterHandler(HTTPMethod method, const HTTPURI& uri,
                                            HTTPMethodHandler handler)
{
    if (method == HTTPMethod::GET)
    {
        auto iter = _getURIHandlers.find(uri);
        if (iter != _getURIHandlers.end())
        {
            return error::ErrorCode::NET_HTTP_REPEATED_URI;
        }

        auto registeredHandler      = std::make_shared<RegisteredHandler>();
        registeredHandler->_uri     = uri;
        registeredHandler->_method  = method;
        registeredHandler->_handler = handler;

        _getURIHandlers[uri] = registeredHandler;
        return error::ErrorCode::SUCCESS;
    }

    if (method == HTTPMethod::POST)
    {
        auto iter = _postURIHandlers.find(uri);
        if (iter != _postURIHandlers.end())
        {
            return error::ErrorCode::NET_HTTP_REPEATED_URI;
        }

        auto registeredHandler      = std::make_shared<RegisteredHandler>();
        registeredHandler->_uri     = uri;
        registeredHandler->_method  = method;
        registeredHandler->_handler = handler;

        _postURIHandlers[uri] = registeredHandler;
        return error::ErrorCode::SUCCESS;
    }

    if (method == HTTPMethod::DELETE)
    {
        auto iter = _deleteURIHandlers.find(uri);
        if (iter != _deleteURIHandlers.end())
        {
            return error::ErrorCode::NET_HTTP_REPEATED_URI;
        }

        auto registeredHandler      = std::make_shared<RegisteredHandler>();
        registeredHandler->_uri     = uri;
        registeredHandler->_method  = method;
        registeredHandler->_handler = handler;

        _deleteURIHandlers[uri] = registeredHandler;
        return error::ErrorCode::SUCCESS;
    }

    if (method == HTTPMethod::PUT)
    {
        auto iter = _putURIHandlers.find(uri);
        if (iter != _putURIHandlers.end())
        {
            return error::ErrorCode::NET_HTTP_REPEATED_URI;
        }

        auto registeredHandler      = std::make_shared<RegisteredHandler>();
        registeredHandler->_uri     = uri;
        registeredHandler->_method  = method;
        registeredHandler->_handler = handler;

        _putURIHandlers[uri] = registeredHandler;
        return error::ErrorCode::SUCCESS;
    }

    return error::ErrorCode::NET_HTTP_INVALID_METHOD;
}

void HTTPServer::Run(const std::string& listenIP, uint16_t listenPort)
{
    _listenIP   = listenIP;
    _listenPort = listenPort;

    _base = event_base_new();
    if (!_base)
    {
        throw std::bad_alloc();
    }

    _httpServer = evhttp_new(_base);
    if (!_httpServer)
    {
        throw std::bad_alloc();
    }

    LOG_DEBUG("http server bind address. {}:{}", _listenIP, _listenPort);

    auto errcode = evhttp_bind_socket(_httpServer, _listenIP.c_str(), _listenPort);
    if (errcode)
    {
        evhttp_free(_httpServer);
        event_base_free(_base);

        _httpServer = nullptr;
        _base       = nullptr;

        auto errmsg = assist::FormatString("can not run http server at the address. %s:%d",
                                           _listenIP.c_str(), _listenPort);
        throw std::system_error(error::ErrorCode::SYSTEM_LIB_EXCEPTION, errmsg);
    }

    evhttp_set_gencb(_httpServer, &HTTPServer::RequestHandler, this);

    LOG_DEBUG("http server started. address:{}:{}", _listenIP, _listenPort);

    int exitedCode = 0;
    do {
        exitedCode = event_base_loop(_base, EVLOOP_NO_EXIT_ON_EMPTY);
    } while (exitedCode != -1);

    LOG_WARN("http server listener run exited. exited code:{}", exitedCode);

    evhttp_free(_httpServer);
    event_base_free(_base);
    _httpServer = nullptr;
    _base       = nullptr;
}

void HTTPServer::Close()
{
    if (!_base)
    {
        return;
    }

    event_base_loopbreak(_base);
    evhttp_free(_httpServer);
    event_base_free(_base);

    _base       = nullptr;
    _httpServer = nullptr;
}

void HTTPServer::RequestHandler(evhttp_request* req, void* arg)
{
    HTTPServer* httpServer = static_cast<HTTPServer*>(arg);
    if (!httpServer)
    {
        return;
    }

    auto method = evhttp_request_get_command(req);
    switch (method)
    {
    case EVHTTP_REQ_GET:
        httpServer->GetRequestHandler(req);
        break;
    case EVHTTP_REQ_POST:
        httpServer->PostRequestHandler(req);
        break;
    case EVHTTP_REQ_DELETE:
        httpServer->DeleteRequestHandler(req);
        break;
    case EVHTTP_REQ_PUT:
        httpServer->PutRequestHandler(req);
        break;
    default:
        httpServer->UnsupportedRequestHandler(req);
        break;
    }
}

void HTTPServer::UnsupportedRequestHandler(evhttp_request* req)
{
    auto buffer = evbuffer_new();
    if (!buffer)
    {
        auto uri = evhttp_request_get_uri(req);
        LOG_ERROR("failed to create response buffer. uri:{}", uri);
        return;
    }

    std::string response = "Unsupported HTTP Method!";
    evbuffer_add_printf(buffer, "%s", response.c_str());
    auto headers = evhttp_request_get_output_headers(req);
    evhttp_add_header(headers, "Content-Type", "application/json; charset=UTF-8");
    evhttp_send_reply(req, HTTP_NOTIMPLEMENTED, "Method Not Allowed", buffer);
    evbuffer_free(buffer);
}

void HTTPServer::GetRequestHandler(evhttp_request* req)
{
    auto buffer = evbuffer_new();
    if (!buffer)
    {
        auto uri = evhttp_request_get_uri(req);
        LOG_ERROR("failed to create response buffer. uri:{}", uri);
        return;
    }

    std::string response = "Unsupported HTTP Method!";
    evbuffer_add_printf(buffer, "%s", "hello world");
    auto headers = evhttp_request_get_output_headers(req);
    evhttp_add_header(headers, "Content-Type", "application/json; charset=UTF-8");
    evhttp_send_reply(req, HTTP_NOTIMPLEMENTED, "Method Not Allowed", buffer);
    evbuffer_free(buffer);
}

void HTTPServer::PostRequestHandler(evhttp_request* req)
{
    auto uri = evhttp_request_get_uri(req);
    if (!uri)
    {
        LOG_WARN("invlaid post request. uri:{}", uri);
        evhttp_send_error(req, HTTP_INTERNAL, 0);
        return;
    }

    auto methodHandlerIter = _postURIHandlers.find(uri);
    if (methodHandlerIter == _postURIHandlers.end())
    {
        LOG_WARN("invlaid post request. uri:{}", uri);
        evhttp_send_error(req, HTTP_INTERNAL, 0);
        return;
    }

    auto inputBuffer     = evhttp_request_get_input_buffer(req);
    auto inputBufferSize = evbuffer_get_length(inputBuffer);

    std::string postData;
    if (inputBufferSize > 0)
    {
        char* data = (char*)malloc(inputBufferSize + 1);
        evbuffer_remove(inputBuffer, data, inputBufferSize);
        data[inputBufferSize] = '\0';
        postData              = data;
        delete[] data;
    }

    auto       registeredHandler = methodHandlerIter->second;
    Parameters inParameters, outParameters;

    int         status = 200;
    std::string response;
    registeredHandler->_handler(inParameters, outParameters, status, response);

    auto outHeaders = evhttp_request_get_output_headers(req);
    if (!outHeaders)
    {
        LOG_ERROR("cat not get output header. request uri:{}", uri);
        evhttp_send_error(req, HTTP_INTERNAL, 0);
        return;
    }

    for (auto iter : outParameters._headerParameters)
    {
        evhttp_add_header(outHeaders, iter.first.c_str(), iter.second.c_str());
    }

    auto responseBuffer = evbuffer_new();
    if (!responseBuffer)
    {
        evhttp_send_error(req, HTTP_INTERNAL, 0);
        return;
    }

    evbuffer_add_printf(responseBuffer, "%s", response.c_str());
    evhttp_send_reply(req, HTTP_OK, "OK", responseBuffer);
    evbuffer_free(responseBuffer);
}

void HTTPServer::DeleteRequestHandler(evhttp_request* req)
{
}

void HTTPServer::PutRequestHandler(evhttp_request* req)
{
}

} // namespace net
} // namespace viper

