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

#ifndef _VIPER_NET_HTTP_CONTEXT_H_
#define _VIPER_NET_HTTP_CONTEXT_H_

#include <functional>
#include <map>
#include <memory>
#include <string>

namespace viper {
namespace net {

using HTTPURI   = std::string;
using Parameter = std::map<std::string, std::string>;

enum class HTTPMethod
{
    GET,
    POST,
    PUT,
    DELETE
};

struct Parameters
{
    Parameter _headerParameters;
    Parameter _queryParamters;
    Parameter _pathParameters;
};

using HTTPMethodHandler = std::function<void(const Parameters& inParameters,
                                             Parameters&       outParameters,
                                             int& status, std::string& response)>;

struct RegisteredHandler
{
    std::string       _uri;
    HTTPMethod        _method;
    HTTPMethodHandler _handler;
};

using RegisteredHandlerPtr = std::shared_ptr<RegisteredHandler>;

} // namespace net
} // namespace viper

#endif

