#include "core/net/tcp_server.h"
#include "core/error/error.h"
#include "core/log/log.h"
#include "core/net/tcp_handler.h"

#include <event2/util.h>

#include <cstring>
#include <netdb.h>
#include <string>

namespace viper {
namespace net {

TCPServer::TCPServer(const std::string& listenAddress, uint16_t port, int threadCount)
{
    _listenAddress = listenAddress;
    _listenPort    = port;
    _threadCount   = threadCount;
}
TCPServer::~TCPServer() {}

void TCPServer::AcceptCallback(struct evconnlistener* listener, evutil_socket_t fd,
                               sockaddr* address, int socklen, void* ctx)
{
    TCPServer* server = reinterpret_cast<TCPServer*>(ctx);

    if (server->_handlers.empty())
    {
        LOG_ERROR("do not set tcp server handler");
        return;
    }

    auto handlerIndex = server->_handlerIndex++ % server->_handlers.size();
    auto handler      = server->_handlers.at(handlerIndex);
    handler->BindConnection(fd, address, socklen);
}

void TCPServer::SetTimeout(int timeoutSec)
{
    _timeoutSec = timeoutSec;
}

void TCPServer::SetCallback(TCPHandlerCallbackFunctor functor)
{
    _functor = functor;
}

std::error_code TCPServer::Run()
{
    for (int i = 0; i < _threadCount; ++i)
    {
        auto handler = std::make_shared<TCPHandler>();
        handler->SetTimeout(_timeoutSec);
        handler->SetCallback(_functor);
        auto errcode = handler->Start();
        if (!error::IsSuccess(errcode))
        {
            LOG_ERROR("failed to create tcp server handler. errcode:{}", errcode.value());
            return errcode;
        }

        _handlers.push_back(handler);
    }

    _base = event_base_new();
    if (!_base)
    {
        LOG_ERROR("failed to create event base");
        return error::ErrorCode::SYSTEM_LIB_EXCEPTION;
    }

    evutil_addrinfo hints;
    memset(&hints, 0, sizeof(evutil_addrinfo));
    hints.ai_family   = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags    = AI_PASSIVE;

    evutil_addrinfo* serviceInfo = nullptr;

    auto port = std::to_string(_listenPort);
    if (evutil_getaddrinfo(_listenAddress.c_str(), port.c_str(), &hints, &serviceInfo))
    {
        LOG_ERROR("failed to get the service info. listen address:{}, listen port:{}", _listenAddress, port);
        event_base_free(_base);
        _base = nullptr;
        return error::ErrorCode::SYSTEM_LIB_EXCEPTION;
    }

    evutil_addrinfo* p = nullptr;
    for (p = serviceInfo; p != nullptr; p = p->ai_next)
    {
        _listener = evconnlistener_new_bind(_base, AcceptCallback, this,
                                            LEV_OPT_CLOSE_ON_FREE | LEV_OPT_REUSEABLE, -1,
                                            p->ai_addr, p->ai_addrlen);
        if (_listener)
        {
            break;
        }
    }
    freeaddrinfo(serviceInfo);

    if (!_listener)
    {
        LOG_ERROR("failed to create listener");

        event_base_free(_base);
        _base = nullptr;
        freeaddrinfo(serviceInfo);
        return error::ErrorCode::SYSTEM_LIB_EXCEPTION;
    }

    // start the event loop
    int exitedCode = 0;
    do {
        exitedCode = event_base_loop(_base, EVLOOP_NO_EXIT_ON_EMPTY);
    } while (exitedCode != -1);

    LOG_WARN("tcp server listener run exited. exited code:{}", exitedCode);

    evconnlistener_free(_listener);
    event_base_free(_base);

    _base     = nullptr;
    _listener = nullptr;

    return error::ErrorCode::SUCCESS;
}

std::error_code TCPServer::Close()
{
    if (!_base)
    {
        return error::ErrorCode::SUCCESS;
    }

    for (auto& handler : _handlers)
    {
        auto errcode = handler->Stop();
        if (!error::IsSuccess(errcode))
        {
            LOG_WARN("failed to stop the tcp server handler. errcode:{}", errcode.value());
        }
    }
    _handlers.clear();

    event_base_loopbreak(_base);
    return error::ErrorCode::SUCCESS;
}

} // namespace net
} // namespace viper

