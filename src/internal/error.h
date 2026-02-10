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

#ifndef _VIPER_INTERNAL_ERROR_H_
#define _VIPER_INTERNAL_ERROR_H_

#include "core/error/error.h"

#include <climits>
#include <cstdint>

namespace viper {
namespace internal {

enum class ErrorCode : int32_t
{
    UNKNOWN = (int32_t)viper::error::ErrorCode::UNKNOWN,
    ERROR   = (int32_t)viper::error::ErrorCode::ERROR,
    SUCCESS = (int32_t)viper::error::ErrorCode::SUCCESS,
    MAX     = INT_MAX,
};

class ErrorCodeCategory final : public std::error_category
{
public:
    static ErrorCodeCategory& Instance();

public:
    const char* name() const noexcept override;
    std::string message(int code) const override;
};

std::error_code make_error_code(ErrorCode e);
bool            IsSuccess(const std::error_code& ec);
std::string     ToString(int ec);

} // namespace internal
} // namespace viper

namespace std {
template <>
struct is_error_code_enum<viper::internal::ErrorCode> : std::true_type
{
};

} // namespace std

#endif

