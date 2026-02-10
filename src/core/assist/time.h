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

#ifndef _VIPER_CORE_ASSIST_TIME_H_
#define _VIPER_CORE_ASSIST_TIME_H_

#include <cstdint>
#include <ctime>
#include <string>

namespace viper {
namespace assist {

/**
 * @brief Sleep sleep some seconds
 *
 * @param secs second value
 */
void Sleep(uint32_t secs);

/**
 * @brief MilliSleep sleep some milliseconds
 *
 * @param milliSecs millisecond value
 */
void MilliSleep(uint32_t milliSecs);

/**
 * @brief Timezone return the current timezone value
 *
 * @return int timezone value
 */
int Timezone();

/**
 * @brief TimestampSecond return the second timestamp
 *
 * @return uint32_t second timestamp
 */
uint32_t TimestampSecond();

/**
 * @brief TimestampMillisecond return the millisecond timestamp
 *
 * @return uint64_t millisecond timestamp
 */
uint64_t TimestampMillisecond();

/**
 * @brief uint64_t TimestampMicrosecond return the microsecond timestamp
 *
 * @return uint64_t microsecond timestamp
 */
uint64_t TimestampMicrosecond();

/**
 * @brief uint64_t TimestampTickCountSecond return the tick count second timestamp
 *
 * @return uint64_t tick count second
 */
uint64_t TimestampTickCountSecond();

/**
 * @brief TimestampTickCountMillisecond return the tick count millisecond timestamp
 *
 * @return uint64_t tick count millisecond
 */
uint64_t TimestampTickCountMillisecond();

/**
 * @brief TimestampTickCountMicrosecond return the tick count microsecond timestamp
 *
 * @return uint64_t tick count microsecond
 */
uint64_t TimestampTickCountMicrosecond();

/**
 * @brief TimeToString format time to string
 *
 * @param time the target time
 * @return std::string string time value
 */
std::string TimeToString(std::time_t time);
std::string TimeToString(std::time_t time, const std::string& formatter);

} // namespace assist
} // namespace viper

#endif
