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

#ifndef _VIPER_CORE_ASSIST_STRING_H_
#define _VIPER_CORE_ASSIST_STRING_H_

#include <cstdint>
#include <cstdio>
#include <sstream>
#include <string>
#include <tuple>
#include <vector>

namespace viper {
namespace assist {

template <typename T>
void FormatStringImp(std::ostringstream& oss, const std::string& format, const T& arg)
{
    oss << arg;
}

template <typename Tuple, size_t... I>
void FormatStringImp(std::ostringstream& oss, const std::string& format, const Tuple& args, std::index_sequence<I...>)
{
    (..., FormatStringImp(oss, format, std::get<I>(args)));
}

template <typename... Args>
std::string FormatString(const std::string& format, Args... args)
{
    std::ostringstream oss;
    FormatStringImp(oss, format, std::make_tuple(args...), std::index_sequence_for<Args...>{});

    std::string result;
    result.resize(format.size() + oss.str().size());

    std::snprintf(result.data(), result.length(), format.c_str(), args...);

    // should return const char point to remove the more zero value
    return result.c_str();
}

/**
 * @brief IsNumber check the value is number
 *
 * @param val the source value
 * @return true is number type
 * @return false is not number type
 */
bool IsNumber(const std::string& val);

/**
 * @brief StrCompare compare the val1 and val2
 *
 * @param val1 the source val1
 * @param val2 the source val2
 * @param ignoreCase true: ignore the case, false: do not ignore case
 * @return int    0: val1 is equal to val2
 *              < 0: val1 is less than val2
 *              > 0: val1 is more than val2
 */
int Comapre(const std::string& val1, const std::string& val2, bool ignoreCase = false);

/**
 * @brief Replace replace the oldstr with newstr in value
 *
 * @param value the source value
 * @param oldstr the oldstr value
 * @param newstr the new value
 */
void Replace(std::string& value, const std::string& oldstr, const std::string& newstr);

/**
 * @brief Split split the value to elements
 *
 * @param value the source value
 * @param delimiter the delimiter value
 * @param elements the splited elements
 */
void Split(const std::string& value, const std::string& delimiter, std::vector<std::string>& elements);

/**
 * @brief TrimRight trim the sequence from the value right
 *
 * @param value the source value
 * @param sequence the character sequence
 */
void TrimRight(std::string& value, const std::string& sequence = " \t\n\r");

/**
 * @brief TrimLeft trim the sequence from the value left
 *
 * @param value the source value
 * @param sequence the character sequence
 */
void TrimLeft(std::string& value, const std::string& sequence = " \t\n\r");

/**
 * @brief Trim trim the sequence from the whole value
 *
 * @param valuen the source value
 * @param sequence the character sequence
 */
void Trim(std::string& value, const std::string& sequence = " \t\n\r");

/**
 * @brief ToString convert the std::vector to string
 *
 * @param elements the source elements
 * @param delimiter the string delimiter
 * @return std::string the joined string
 */
std::string ToString(const std::vector<std::string>& elements, const std::string& delimiter);

/**
 * @brief ToBool convert string value to bool value
 *
 * @param value the source value
 * @param defaultValue if converted failed, return this value
 * @return true the source value is true
 * @return false the source value is false
 */
bool ToBool(const std::string& value, bool defaultValue = false);

/**
 * @brief Hash caculate the string value's hash
 *
 * @param data the inputed data
 * @return uint16_t hash value
 */
uint16_t Hash(const std::string& data);

} // namespace assist
} // namespace viper

#endif
