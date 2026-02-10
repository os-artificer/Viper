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

#ifndef _VIPER_CORE_ASSIST_FILE_H_
#define _VIPER_CORE_ASSIST_FILE_H_

#include "core/error/error.h"

#include <cstdint>
#include <string>
#include <vector>

namespace viper {
namespace assist {

/**
 * @brief IsFile check if the name is file
 *
 * @param name the target path
 * @return true is file
 * @return false is not file
 */
bool IsFile(const std::string& name);

/**
 * @brief IsDirectory check if the name is directory
 *
 * @param name the target path
 * @return true is directory
 * @return false is not directory
 */
bool IsDirectory(const std::string& name);

/**
 * @brief FileExists check the file or directory exists
 *
 * @param fileName the file path
 * @return true exists
 * @return false not exists
 */
bool FileExists(const std::string& fileName);

/**
 * @brief FileSize return the file size(MB)
 *
 * @param fileName the file path
 * @return int64_t  >= 0 :file size(MB)
 *                   < 0 :make system exception
 */
int64_t FileSize(const std::string& fileName);

/**
 * @brief ReadFullFile read the full content from the file
 *
 * @param fileName the file path
 * @param content the content of the file
 * @return std::error_code Success it's success, other value is error
 */
std::error_code ReadFullFile(const std::string& fileName, std::string& content);

/**
 * @brief DeleteFile delete the file
 *
 * @param fileName the file path
 * @return std::error_code Success it's success, other value is error
 */
std::error_code DeleteFile(const std::string& fileName);

/**
 * @brief DeleteDirectory delete the directory
 *
 * @param fileName the directory path
 * @return std::error_code Success it's success, other value is error
 */
std::error_code DeleteDirectory(const std::string& fileName);

/**
 * @brief RenameFile rename the oldName to newName
 *
 * @param oldName the old file name
 * @param newName the new file name
 * @return std::error_code Success it's success, other value is error
 */
std::error_code RenameFile(const std::string& oldName, const std::string& newName);

/**
 * @brief SetFileUser set the file's user if exists
 *
 * @param fileName the file path
 * @param userName the user
 * @return std::error_code Success for success, others for error
 */
std::error_code SetFileUser(const std::string& fileName, const std::string& userName);

/**
 * @brief SetFilePermission set the file's permission if exists
 *
 * @param fileName the file path
 * @param mode the file permission code
 * @return std::error_code Success for success, others for error
 */
std::error_code SetFilePermission(const std::string& fileName, int mode);

/**
 * @brief ListFile list all files in the directory
 *
 * @param dirName the directiory path
 * @param files the all files
 * @param recursive recursively iterates over all files in a subdirectory
 * @return std::error_code Success for success, others for error
 */
std::error_code ListFile(const std::string& dirName, std::vector<std::string>& files, bool recursive = false);

/**
 * @brief MakeDirectory make directory with given path
 *
 * @param dirPath the directory path
 * @return std::error_code Success for success, others for error
 */
std::error_code MakeDirectory(const std::string& dirPath);

/**
 * @brief JoinPath combine the multi paths
 *
 * @param paths the path in order, will combined together
 * @return std::string the final combined path
 */
std::string JoinPath(const std::vector<std::string>& paths);

/**
 * @brief JoinPath combine the prefix and multi paths
 *
 * @param prefix the prefix path
 * @param paths the paths in order, will be combined together
 * @return std::string the final combined path
 */
std::string JoinPath(const std::string& prefix, const std::vector<std::string>& paths);

/**
 * @brief LockFile lock the file exclusively.
 *
 * @param fileFD the file's fd
 * @return std::error_code Success for success, others for error
 */
std::error_code LockFile(int fileFD);

/**
 * @brief UnlockFile unlock the file
 *
 * @param fileFD the file's fd
 * @return std::error_code Success for success, others for error
 */
std::error_code UnlockFile(int fileFD);

} // namespace assist
} // namespace viper

#endif

