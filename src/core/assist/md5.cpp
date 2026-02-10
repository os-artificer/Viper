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

#include "core/assist/md5.h"

#include <openssl/evp.h>
#include <openssl/md5.h>

#include <iomanip>
#include <iostream>
#include <sstream>

namespace viper {
namespace assist {

std::string MD5::MD5FromString(const std::string& content)
{
    unsigned char digest[MD5_DIGEST_LENGTH] = {0};
    EVP_MD_CTX*   ctx                       = EVP_MD_CTX_new();
    EVP_MD_CTX_init(ctx);

    const EVP_MD* md = EVP_md5();

    EVP_DigestInit_ex(ctx, md, nullptr);
    EVP_DigestUpdate(ctx, content.c_str(), content.size());
    EVP_DigestFinal(ctx, digest, nullptr);

    std::stringstream ss;
    for (int i = 0; i < MD5_DIGEST_LENGTH; ++i)
    {
        ss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(digest[i]);
    }
    EVP_MD_CTX_free(ctx);

    return ss.str();
}

std::string MD5::MD5FromFile(const std::string& fileName)
{

    FILE* fp = fopen(fileName.c_str(), "rb");
    if (!fp)
    {
        return "";
    }

    int   len  = 0;
    char* data = (char*)malloc(10 * 1024 * 1024);
    if (!data)
    {
        fclose(fp);
        return "";
    }

    unsigned char digest[MD5_DIGEST_LENGTH] = {0};
    EVP_MD_CTX*   ctx                       = EVP_MD_CTX_new();
    EVP_MD_CTX_init(ctx);

    const EVP_MD* md = EVP_md5();
    EVP_DigestInit_ex(ctx, md, nullptr);

    while (0 != (len = fread(data, 1, 10 * 1024 * 1024, fp)))
    {
        EVP_DigestUpdate(ctx, data, len);
    }

    EVP_DigestFinal(ctx, digest, nullptr);

    std::stringstream ss;
    for (int i = 0; i < MD5_DIGEST_LENGTH; ++i)
    {
        ss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(digest[i]);
    }

    EVP_MD_CTX_free(ctx);

    free(data);
    fclose(fp);

    return ss.str();
}

} // namespace assist
} // namespace viper
