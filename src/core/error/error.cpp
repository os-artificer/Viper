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

#include "core/error/error.h"

#include <system_error>

namespace viper {
namespace error {

ErrorCodeCategory& ErrorCodeCategory::Instance()
{
    static ErrorCodeCategory instance;
    return instance;
}

const char* ErrorCodeCategory::name() const noexcept
{
    return "ylg-core-error";
}

std::string ErrorCodeCategory::message(int code) const
{
    std::string errMsg;
    switch (static_cast<ErrorCode>(code))
    {
    default:
        errMsg = std::to_string(code) + ": unknown error code";
    }
    return errMsg;
}

std::error_code make_error_code(ErrorCode e)
{
    return {static_cast<int>(e), ErrorCodeCategory::Instance()};
}

bool IsSuccess(const std::error_code& ec)
{
    if (ec.value() == (int)ErrorCode::SUCCESS)
    {
        return true;
    }

    return false;
}

std::string ToString(int ec)
{
    return std::system_category().message(ec);
}

} // namespace error
} // namespace viper

