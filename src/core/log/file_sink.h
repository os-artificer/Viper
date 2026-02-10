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

#ifndef _VIPER_CORE_LOG_FILE_SINK_H_
#define _VIPER_CORE_LOG_FILE_SINK_H_

#include "core/log/file.h"

#include <spdlog/common.h>
#include <spdlog/details/log_msg.h>
#include <spdlog/sinks/base_sink.h>

#include <cstddef>
#include <mutex>

namespace viper {
namespace log {

class FileSink final : public spdlog::sinks::base_sink<std::mutex>
{
public:
    FileSink(const std::string& path, spdlog::filename_t fileName, std::size_t max,
             std::size_t maxCount, bool rotateOnOpen = false,
             const spdlog::file_event_handlers& handlers = {});

public:
    /**
     * @brief return the log _file name
     *
     * @return spdlog::filename_t
     */
    spdlog::filename_t FileName();

protected:
    /**
     * @brief sink_it_ override the sink operation
     *
     * @param message the log message
     */
    void sink_it_(const spdlog::details::log_msg& message) override;

    /**
     * @brief flush_ override the sink operaion
     *
     */
    void flush_() override;

private:
    /**
     * @brief Rotate rotate the log _file base on the rule(max size and _file count)
     *
     */
    void Rotate();

    /**
     * @brief RenameFile rename the srcName to dstName, delete the dstName if exists
     *
     * @param srcName the source _file name
     * @param dstName the dest _file name
     * @return true success
     * @return false failure
     */
    bool RenameFile(const spdlog::filename_t& srcName, const spdlog::filename_t& dstName);

private:
    std::string        _basePath;
    spdlog::filename_t _baseFileName;
    File               _file;
    std::size_t        _maxSize      = 0;
    std::size_t        _maxFileCount = 0;
    std::size_t        _currentSize  = 0;
};

} // namespace log
} // namespace viper

#endif
