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

#ifndef _VIPER_ERROR_ERROR_H_
#define _VIPER_ERROR_ERROR_H_

#include "core/assist/macro.h"
#include "core/log/log.h"

#include <climits>
#include <cstdint>
#include <system_error>

namespace viper {
namespace error {

enum class ErrorCode : int32_t
{
    // base error code
    UNKNOWN = -2,
    ERROR   = -1,
    SUCCESS = 0,

    // system error code
    SYSTEM_NOT_FOUND,
    SYSTEM_LIB_EXCEPTION,
    SYSTEM_MEM_EXCEPTION,
    SYSTEM_TRY_AGAIN,
    SYSTEM_INVALID_USER,

    // net error code
    NET_MESSAGE_TOO_LARGE,
    NET_INVALID_MAGIC,
    NET_SEND_FAILED,
    NET_DISCONNECTED,
    NET_HTTP_INVALID_METHOD,
    NET_HTTP_REPEATED_URI,
    NET_HTTP_RESPOND_FAILED,

    // application error code
    APP_CONFIGURATION_INVALID,
    APP_DISCOVERY_CREATE_LEASE_FAILED,
    APP_DISCOVERY_REGISTER_SERVICE_FAILED,
    APP_DISCOVERY_WATCHER_REPEATED,

    // assist error code
    QUEUE_OVERFLOW,
    FILE_EXCEPTION,
    INVALID_PARAMETER,

    // max code
    MAX = USHRT_MAX,
    CUSTOM_BEGIN,
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

} // namespace error
} // namespace viper

namespace std {
template <>
struct is_error_code_enum<viper::error::ErrorCode> : std::true_type
{
};

} // namespace std

#endif

