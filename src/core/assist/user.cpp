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

#include "core/assist/user.h"
#include "core/assist/memory.h"
#include "core/error/error.h"
#include "core/log/log.h"

#include <pwd.h>
#include <unistd.h>

namespace viper {
namespace assist {

bool UserExists(const std::string& userName)
{
    if (userName.empty())
    {
        return false;
    }

    struct passwd  pwd;
    struct passwd* res = nullptr;
    char           buffer[1024];

    ZeroMemory(buffer, sizeof(buffer));

    return getpwnam_r(userName.c_str(), &pwd, buffer, sizeof(buffer), &res) == 0 && res != nullptr;
}

std::error_code GetUserUidGid(const std::string& userName, int& uid, int& gid)
{
    struct passwd  pwd;
    struct passwd* res = nullptr;
    char           buffer[1024];

    ZeroMemory(buffer, sizeof(buffer));

    auto isOK = getpwnam_r(userName.c_str(), &pwd, buffer, sizeof(buffer), &res) && res != nullptr;
    if (!isOK)
    {
        LOG_ERROR("can not get uid and gid. errmsg({})", error::ToString(errno));
        return error::ErrorCode::ERROR;
    }

    uid = res->pw_uid;
    gid = res->pw_gid;

    return error::ErrorCode::SUCCESS;
}

std::error_code GetUserUidGid(const std::string& userName, int& uid, int& gid, std::string& homeDirectory, std::string& shell)
{
    struct passwd  pwd;
    struct passwd* res = nullptr;
    char           buffer[1024];

    ZeroMemory(buffer, sizeof(buffer));

    auto isOK = getpwnam_r(userName.c_str(), &pwd, buffer, sizeof(buffer), &res) && res != nullptr;
    if (!isOK)
    {
        LOG_ERROR("can not get uid and gid. errmsg({})", error::ToString(errno));
        return error::ErrorCode::ERROR;
    }

    uid           = res->pw_uid;
    gid           = res->pw_gid;
    shell         = res->pw_shell;
    homeDirectory = res->pw_dir;

    return error::ErrorCode::SUCCESS;
}

std::error_code SetUserUidGid(int uid, int gid)
{
    if (setregid(gid, gid))
    {
        LOG_ERROR("can not set gid. errmsg({})", error::ToString(errno));
        return error::ErrorCode::ERROR;
    }

    if (setreuid(uid, uid))
    {
        LOG_ERROR("can not get uid. errmsg({})", error::ToString(errno));
        return error::ErrorCode::ERROR;
    }

    return error::ErrorCode::SUCCESS;
}

} // namespace assist
} // namespace viper

