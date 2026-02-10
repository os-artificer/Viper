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

#include "core/log/file_sink.h"
#include "core/assist/file.h"
#include "core/assist/time.h"
#include "core/error/error.h"
#include "core/log/file.h"

#include <spdlog/common.h>
#include <spdlog/details/os.h>
#include <spdlog/fmt/bundled/format.h>

#include <cerrno>
#include <cstdio>
#include <ctime>
#include <mutex>
#include <vector>

namespace viper {
namespace log {

FileSink::FileSink(const std::string& path, spdlog::filename_t fileName, std::size_t max,
                   std::size_t maxCount, bool rotateOnOpen,
                   const spdlog::file_event_handlers& handler)
    : _file(handler)
{
    _basePath     = path;
    _baseFileName = fileName;
    _maxSize      = max;
    _maxFileCount = maxCount;

    _file.Open(assist::JoinPath({_basePath, fileName}));
    _currentSize = _file.Size();

    if (rotateOnOpen && _currentSize > 0)
    {
        Rotate();
        _currentSize = 0;
    }
}

spdlog::filename_t FileSink::FileName()
{
    std::lock_guard<std::mutex> lock(base_sink<std::mutex>::mutex_);
    return _file.Name();
}

void FileSink::sink_it_(const spdlog::details::log_msg& message)
{
    spdlog::memory_buf_t formatted;
    spdlog::sinks::base_sink<std::mutex>::formatter_->format(message, formatted);

    auto newSize = _currentSize + formatted.size();

    if (newSize > _maxSize)
    {
        _file.Flush();
        auto fileSize = _file.Size();

        if (fileSize > 0 || (-1 == fileSize && 0 != _currentSize))
        {
            Rotate();

            // reset the current file size after rotate
            newSize      = formatted.size();
            _currentSize = 0;
        }
    }

    if (_file.Write(formatted))
    {
        _currentSize = newSize;
    }
}

void FileSink::flush_()
{
    _file.Flush();
}

void FileSink::Rotate()
{
    _file.Close();

    spdlog::filename_t fileName, fileExt;
    std::tie(fileName, fileExt) = File::SplitByExt(fileName);

    spdlog::filename_t source = spdlog::fmt_lib::format(SPDLOG_FILENAME_T("{}/{}"), _basePath, _baseFileName);
    if (spdlog::details::os::path_exists(source))
    {
        spdlog::filename_t target = spdlog::fmt_lib::format(SPDLOG_FILENAME_T("{}/{}.{}{}"),
                                                            _basePath, _baseFileName,
                                                            assist::TimeToString(std::time(nullptr), "%Y%m%d%H%M%S"), fileExt);

        if (!RenameFile(source, target))
        {
            spdlog::details::os::sleep_for_millis(100);
            if (!RenameFile(source, target))
            {
                _file.Reopen(true);
                _currentSize = 0;

                spdlog::throw_spdlog_ex("failed to rename " +
                                            spdlog::details::os::filename_to_str(source) +
                                            " to " + spdlog::details::os::filename_to_str(target),
                                        errno);
            }
        }
    }

    // scan log files
    std::vector<std::string> logFiles;
    std::vector<std::string> scanFiles;

    auto ec = assist::ListFile(_basePath, scanFiles);

    if (!error::IsSuccess(ec))
    {
        fprintf(stderr, "%s", ec.message().c_str());
        return;
    }

    for (auto iter = scanFiles.begin(); iter != scanFiles.end(); ++iter)
    {
        spdlog::filename_t scannedBaseName, scannedExt;
        std::tie(scannedBaseName, scannedExt) = File::SplitByExt(*iter);

        if (scannedExt != fileExt)
        {
            continue;
        }

        std::tie(scannedBaseName, scannedExt) = File::SplitByExt(scannedBaseName);
        if (scannedBaseName == _baseFileName && !scannedExt.empty())
        {
            logFiles.push_back(spdlog::fmt_lib::format(SPDLOG_FILENAME_T("{}/{}"), _baseFileName, *iter));
        }
    }

    std::sort(logFiles.begin(), logFiles.end());

    // delete the oldest log file
    if (logFiles.size() > _maxFileCount)
    {
        auto needDeleteFileCount = logFiles.size() - _maxFileCount;
        for (auto i = 0; i < needDeleteFileCount && i < logFiles.size(); ++i)
        {
            spdlog::details::os::remove(logFiles.at(i));
        }
    }

    _file.Reopen(true);
}

bool FileSink::RenameFile(const spdlog::filename_t& srcName, const spdlog::filename_t& dstName)
{
    return spdlog::details::os::rename(srcName, dstName);
}

} // namespace log
} // namespace viper

