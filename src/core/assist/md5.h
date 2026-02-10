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

#ifndef _VIPER_CORE_ASSIST_MD5_H_
#define _VIPER_CORE_ASSIST_MD5_H_

#include <string>

#include <openssl/evp.h>
#include <openssl/md5.h>

namespace viper {
namespace assist {

class MD5 final
{
public:
    static std::string MD5FromString(const std::string& content);
    static std::string MD5FromFile(const std::string& fileName);
};

} // namespace assist
} // namespace viper

#endif
