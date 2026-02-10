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

#ifndef _VIPER_TEXT_STRINGS_H_
#define _VIPER_TEXT_STRINGS_H_

#include <string>
#include <string_view>
#include <vector>

namespace viper::text {

// Lightweight string utilities used across the parser and examples.

/// Return true if `sv` begins with `prefix`.
bool StartWith(std::string_view sv, std::string_view prefix);

/// Return true if `sv` contains the substring `substr`.
bool Contains(std::string_view sv, std::string_view substr);

/// Split `sv` using a single character delimiter. Empty tokens are preserved.
std::vector<std::string> Split(std::string_view sv, char delimiter);

/// Split `str` using a string delimiter.
std::vector<std::string> Split(std::string_view str, std::string_view delimiter);

/// Return a lower-case copy of `str`.
std::string ToLower(const std::string& str);

/**
 * @brief Convert `str` to boolean using common textual rules.
 *
 * Recognized inputs (case-insensitive):
 *  - "true" or "1"  => true
 *  - "false" or "0" => false
 *
 * Throws `std::invalid_argument` for unrecognized values.
 */
bool ToBool(const std::string& str);

} // namespace viper::text

#endif
