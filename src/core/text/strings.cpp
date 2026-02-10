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

#include "core/text/strings.h"

#include <algorithm>
#include <cctype>
#include <stdexcept>

namespace viper::text {

bool StartWith(std::string_view sv, std::string_view prefix)
{
    if (prefix.length() > sv.length())
    {
        return false;
    }

    return std::equal(prefix.begin(), prefix.end(), sv.begin());
}

bool Contains(std::string_view sv, std::string_view substr)
{
    return sv.find(substr) != std::string_view::npos;
}

std::vector<std::string> Split(std::string_view sv, char delimiter)
{
    std::vector<std::string> result;
    size_t                   start = 0;

    for (size_t end = sv.find(delimiter);
         end != std::string_view::npos;
         end = sv.find(delimiter, start))
    {
        result.emplace_back(sv.substr(start, end - start));
        start = end + 1;
    }

    if (start < sv.size())
    {
        result.emplace_back(sv.substr(start));
    }

    return result;
}

std::vector<std::string> Split(std::string_view str, std::string_view delimiter)
{
    std::vector<std::string> result;
    size_t                   start        = 0;
    size_t                   delimiterLen = delimiter.length();

    if (delimiterLen == 0)
    {
        result.push_back(str.data());
        return result;
    }

    for (size_t end = str.find(delimiter);
         end != std::string_view::npos;
         end = str.find(delimiter, start))
    {
        result.emplace_back(str.substr(start, end - start));
        start = end + delimiterLen;
    }

    if (start < str.size())
    {
        result.emplace_back(str.substr(start));
    }

    return result;
}

std::string ToLower(const std::string& str)
{
    std::string result;
    for (char c : str)
    {
        result += static_cast<char>(tolower(static_cast<unsigned char>(c)));
    }
    return result;
}

bool ToBool(const std::string& str)
{
    std::string lowerStr = ToLower(str);

    if (lowerStr == "true" || lowerStr == "1")
    {
        return true;
    }
    else if (lowerStr == "false" || lowerStr == "0")
    {
        return false;
    }
    else
    {
        throw std::invalid_argument("invalid data: " + str);
    }
}
} // namespace viper::text
