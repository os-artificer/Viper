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

#ifndef _VIPER_CORE_LOG_H_
#define _VIPER_CORE_LOG_H_

#include <spdlog/async.h>
#include <spdlog/async_logger.h>
#include <spdlog/common.h>
#include <spdlog/details/thread_pool.h>
#include <spdlog/spdlog.h>

#include <cstdio>
#include <memory>
#include <string>

namespace viper {
namespace log {

// clang-format off

#define LEVEL_DEBUG "debug"
#define LEVEL_INFO  "info"
#define LEVEL_WARN  "warn"
#define LEVEL_ERROR "error"

// clang-format on

struct LogConfig
{
    std::string _level       = LEVEL_INFO;
    std::string _path        = "log";
    int         _fileSizeMB  = 100;
    int         _fileCount   = 5;
    int         _flushMS     = 100;
    std::string _pattern     = "[%L%m%d %H:%M:%S.%f %P-%t %s:%#] %v";
    int         _threadCount = 1;
    int         _queueSize   = 36480;
};

class Logger final
{
public:
    Logger();
    ~Logger();

public:
    static Logger& Instance();

public:
    std::error_code                       Init(const std::string& name, const LogConfig& cfg);
    void                                  Close();
    bool                                  IsOpen();
    std::shared_ptr<spdlog::async_logger> AsyncLogger();

private:
    spdlog::level::level_enum Parse(const std::string& level);
    bool                      RedirectSTD(const std::string& name, const LogConfig& cfg);

private:
    std::shared_ptr<spdlog::details::thread_pool> _threadPoolPtr;
    std::shared_ptr<spdlog::async_logger>         _asyncLogger;
    std::FILE*                                    _stdOut = nullptr;
    std::FILE*                                    _stdErr = nullptr;
    std::atomic_bool                              _isOpen = false;
};

} // namespace log
} // namespace viper

#define LOG_STD_FORMAT(fileName, lineNum, funcName, fmt, ...) \
    do {                                                      \
        std::string format;                                   \
        format.append(fileName);                              \
        format.append(",");                                   \
        format.append(std::to_string(lineNum));               \
        format.append(",");                                   \
        format.append(funcName);                              \
        format.append(" ");                                   \
        format.append(fmt);                                   \
        format.append("\n");                                  \
        std::fprintf(stdout, format.c_str(), ##__VA_ARGS__);  \
    } while (0)

#define LOG_FORMAT(level, fileName, lineNum, funcName, fmt, ...)                                           \
    do {                                                                                                   \
        auto& inst = viper::log::Logger::Instance();                                                         \
        if (inst.IsOpen())                                                                                 \
        {                                                                                                  \
            auto logger = inst.AsyncLogger();                                                              \
            if (logger && logger->should_log(level))                                                       \
            {                                                                                              \
                logger->log(spdlog::source_loc{fileName, lineNum, funcName}, level, (fmt), ##__VA_ARGS__); \
            }                                                                                              \
        }                                                                                                  \
    } while (0)

#define LOG_FATAL(fmt, ...)                                                                                             \
    do {                                                                                                                \
        auto& inst   = viper::log::Logger::Instance();                                                                    \
        auto  logger = inst.AsyncLogger();                                                                              \
        logger->log(spdlog::source_loc{__FILE__, __LINE__, SPDLOG_FUNCTION}, spdlog::level::err, (fmt), ##__VA_ARGS__); \
        inst.Close();                                                                                                   \
        _exit(1);                                                                                                       \
    } while (0)

#define LOG_STD(fmt, ...) \
    LOG_STD_FORMAT(__FILE__, __LINE__, SPDLOG_FUNCTION, (fmt), ##__VA_ARGS__)

#define LOG_DEBUG(fmt, ...) \
    LOG_FORMAT(spdlog::level::debug, __FILE__, __LINE__, SPDLOG_FUNCTION, (fmt), ##__VA_ARGS__)

#define LOG_INFO(fmt, ...) \
    LOG_FORMAT(spdlog::level::info, __FILE__, __LINE__, SPDLOG_FUNCTION, (fmt), ##__VA_ARGS__)

#define LOG_WARN(fmt, ...) \
    LOG_FORMAT(spdlog::level::warn, __FILE__, __LINE__, SPDLOG_FUNCTION, (fmt), ##__VA_ARGS__)

#define LOG_ERROR(fmt, ...) \
    LOG_FORMAT(spdlog::level::err, __FILE__, __LINE__, SPDLOG_FUNCTION, (fmt), ##__VA_ARGS__)

#endif
