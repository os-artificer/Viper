/**
 * YingLongGeeker is pleased to support the open source community by making this
 * software available. Copyright (C) 2021 YingLongGeeker. All rights reserved.
 * Licensed under the MIT License (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * https://opensource.org/licenses/MIT
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef _VIPER_CORE_ASSIST_HOST_H_
#define _VIPER_CORE_ASSIST_HOST_H_

#include <system_error>

namespace viper {
namespace assist {

/**
 * @brief HostCPUCount return host CPU count
 *
 * @return int CPU count
 */
int HostCPUCount();

/**
 * @brief HostCPUFrequency return host CPU frequency
 *
 * @return double CPU frequency
 */
double HostCPUFrequency();

/**
 * @brief HostCPUUsage return host CPU usage
 *
 * @return double CPU usage
 */
double HostCPUUsage();

/**
 * @brief Hostname return host name
 *
 * @param hostName output host name
 *
 * @return std::string host name
 */
std::error_code Hostname(std::string& hostName);

} // namespace assist
} // namespace viper
#endif
