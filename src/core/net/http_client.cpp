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

#include "core/net/http_client.h"
#include "core/assist/string.h"
#include "core/error/error.h"

#include <curl/curl.h>
#include <curl/easy.h>

#include <new>

namespace viper {
namespace net {

HTTPClient::HTTPClient(const std::string& address)
{
    _address = address;
    _curl    = curl_easy_init();
    if (!_curl)
    {
        throw std::bad_alloc();
    }
}

HTTPClient::~HTTPClient()
{
    if (_curl)
    {
        curl_easy_cleanup(_curl);
        _curl = nullptr;
    }
}

HTTPClient* HTTPClient::ResetHeader()
{
    _header.clear();
    return this;
}

HTTPClient* HTTPClient::SetHeader(const std::string& key, const std::string& value)
{
    _header[key] = value;
    return this;
}

HTTPClient* HTTPClient::SetTimeout(int seconds)
{
    _timeoutSeconds = seconds;
    return this;
}

std::error_code HTTPClient::Put(const HTTPURI& uri, const std::string& request, int& status, std::string& response)
{
    auto address = assist::FormatString("%s/%s", _address.c_str(), uri.c_str());
    curl_easy_setopt(_curl, CURLOPT_URL, address.c_str());
    curl_easy_setopt(_curl, CURLOPT_CUSTOMREQUEST, "PUT");
    curl_easy_setopt(_curl, CURLOPT_POSTFIELDS, request.c_str());
    return Execute(status, response);
}

std::error_code HTTPClient::Delete(const HTTPURI& uri, const std::string& request, int& status, std::string& response)
{
    auto address = assist::FormatString("%s/%s", _address.c_str(), uri.c_str());
    curl_easy_setopt(_curl, CURLOPT_URL, address.c_str());
    curl_easy_setopt(_curl, CURLOPT_CUSTOMREQUEST, "DELETE");
    curl_easy_setopt(_curl, CURLOPT_POSTFIELDS, request.c_str());
    return Execute(status, response);
}

std::error_code HTTPClient::Get(const HTTPURI& uri, int& status, std::string& response)
{
    auto address = assist::FormatString("%s/%s", _address.c_str(), uri.c_str());
    curl_easy_setopt(_curl, CURLOPT_URL, address.c_str());
    curl_easy_setopt(_curl, CURLOPT_HTTPGET, 1);
    return Execute(status, response);
}

std::error_code HTTPClient::Post(const HTTPURI& uri, const std::string& request, int& status, std::string& response)
{
    auto address = assist::FormatString("%s/%s", _address.c_str(), uri.c_str());
    curl_easy_setopt(_curl, CURLOPT_URL, address.c_str());
    curl_easy_setopt(_curl, CURLOPT_POST, 1);
    curl_easy_setopt(_curl, CURLOPT_POSTFIELDS, request.c_str());
    return Execute(status, response);
}

std::size_t HTTPClient::WriteData(void* data, std::size_t size, std::size_t nmemb, void* user)
{
    std::string* content = static_cast<std::string*>(user);
    if (!content || !data)
    {
        return 0;
    }

    std::size_t realsize = size * nmemb;
    content->append((char*)data, realsize);
    return realsize;
}

std::error_code HTTPClient::Execute(int& status, std::string& response)
{
    response.clear();
    status = 0;

    curl_slist* curlList = nullptr;
    for (auto iter = _header.begin(); iter != _header.end(); ++iter)
    {
        if (iter->first.empty() || iter->second.empty())
        {
            continue;
        }

        auto keyValue = assist::FormatString("%s: %s", iter->first.c_str(), iter->second.c_str());
        curlList      = curl_slist_append(curlList, keyValue.c_str());
    }

    curl_easy_setopt(_curl, CURLOPT_READFUNCTION, nullptr);
    curl_easy_setopt(_curl, CURLOPT_WRITEFUNCTION, HTTPClient::WriteData);
    curl_easy_setopt(_curl, CURLOPT_WRITEDATA, (void*)&response);
    curl_easy_setopt(_curl, CURLOPT_NOSIGNAL, 1L);
    curl_easy_setopt(_curl, CURLOPT_IPRESOLVE, CURL_IPRESOLVE_WHATEVER);

    curl_easy_setopt(_curl, CURLOPT_TIMEOUT, _timeoutSeconds);
    curl_easy_setopt(_curl, CURLOPT_TCP_KEEPALIVE, 1L);
    curl_easy_setopt(_curl, CURLOPT_TCP_KEEPIDLE, 300L);

    curl_easy_setopt(_curl, CURLOPT_SSL_VERIFYPEER, false); // disable ssl
    curl_easy_setopt(_curl, CURLOPT_SSL_VERIFYHOST, false);

    CURLcode code = curl_easy_perform(_curl);
    if (CURLE_OK != code)
    {
        curl_slist_free_all(curlList);
        return error::ErrorCode::NET_HTTP_RESPOND_FAILED;
    }

    code = curl_easy_getinfo(_curl, CURLINFO_RESPONSE_CODE, &status);
    if (CURLE_OK != code)
    {
        curl_slist_free_all(curlList);
        return error::ErrorCode::NET_HTTP_RESPOND_FAILED;
    }

    curl_slist_free_all(curlList);

    return error::ErrorCode::SUCCESS;
}

} // namespace net
} // namespace viper

