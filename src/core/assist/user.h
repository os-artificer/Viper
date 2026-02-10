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

#ifndef _VIPER_CORE_ASSIST_USER_H_
#define _VIPER_CORE_ASSIST_USER_H_

#include "core/error/error.h"

#include <string>

namespace viper {
namespace assist {

/**
 * @brief UserExists check the user if it's exists
 *
 * @param userName the user name
 * @return true the user is valid
 * @return false the user is invalid
 */
bool UserExists(const std::string& userName);

/**
 * @brief GetUserUidGid get the user's uid and gid
 *
 * @param userName the user name
 * @param uid the user's uid
 * @param gid the user's gid
 * @return std::error_code Success it's success, other value is error
 */
std::error_code GetUserUidGid(const std::string& userName, int& uid, int& gid);

/**
 * @brief GetUserUidGid get the user's information
 *
 * @param userName the user name
 * @param uid the user ID
 * @param gid the user group ID
 * @param homeDirectory the user home directory
 * @param shell the user shell program
 * @return std::error_code Success it's success, other value is error
 */
std::error_code GetUserUidGid(const std::string& userName, int& uid, int& gid, std::string& homeDirectory, std::string& shell);

/**
 * @brief SetUserUidGid set the user uid and gid for the current process
 *
 * @param uid user ID
 * @param gid group ID
 * @return std::error_code Success it's success, other value is error
 */
std::error_code SetUserUidGid(int uid, int gid);

} // namespace assist
} // namespace viper

#endif

