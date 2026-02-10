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

#ifndef _VIPER_CORE_NET_HTTP_CLIENT_H_
#define _VIPER_CORE_NET_HTTP_CLIENT_H_

#include "core/net/http_context.h"

#include <curl/curl.h>

#include <system_error>

namespace viper {
namespace net {

class HTTPClient final
{
public:
    HTTPClient(const std::string& address);
    ~HTTPClient();

public:
    HTTPClient* ResetHeader();
    HTTPClient* SetHeader(const std::string& key, const std::string& value);
    HTTPClient* SetTimeout(int seconds);

    std::error_code Put(const HTTPURI& uri, const std::string& request, int& status, std::string& response);
    std::error_code Delete(const HTTPURI& uri, const std::string& request, int& status, std::string& response);
    std::error_code Get(const HTTPURI& uri, int& status, std::string& response);
    std::error_code Post(const HTTPURI& uri, const std::string& request, int& status, std::string& response);

private:
    static std::size_t WriteData(void* data, std::size_t size, std::size_t nmemb, void* user);
    std::error_code    Execute(int& status, std::string& response);

private:
    int         _timeoutSeconds = 10;
    CURL*       _curl           = nullptr;
    std::string _address;
    Parameter   _header;
};

} // namespace net
} // namespace viper

#endif

