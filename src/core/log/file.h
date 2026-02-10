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

#ifndef _VIPER_CORE_LOG_FILE_H_
#define _VIPER_CORE_LOG_FILE_H_

#include <spdlog/common.h>

#include <cstdint>
#include <cstdio>

namespace viper {
namespace log {

class File final
{
public:
    File() = default;
    File(const spdlog::file_event_handlers& inHandlers);
    File(const File&)            = delete;
    File& operator=(const File&) = delete;
    ~File();

public:
    static std::tuple<spdlog::filename_t, spdlog::filename_t> SplitByExt(const spdlog::filename_t& fileName);

public:
    void                      Open(const spdlog::filename_t& fileName, bool truncate = false);
    void                      Reopen(bool truncate);
    bool                      Flush();
    void                      Close();
    bool                      Write(const spdlog::memory_buf_t& buffer);
    std::size_t               Size();
    const spdlog::filename_t& Name() const;

private:
    uint32_t                    _openRetries  = 5;
    uint32_t                    _openInterval = 10;
    std::FILE*                  _fd           = nullptr;
    spdlog::filename_t          _name;
    spdlog::file_event_handlers _handlers;
};

} // namespace log
} // namespace viper

#endif
