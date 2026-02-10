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

#ifndef _VIPER_CORE_ASSIST_MATH_H_
#define _VIPER_CORE_ASSIST_MATH_H_

#include <cstdint>
#include <string>

namespace viper {
namespace assist {

/**
 * @brief CRC16 imp according to CCITT standards
 *
 * @param buf the inputed buffer
 * @param len the inputed buffer size
 *
 * @return uint16_t 16 bits hash integer value
 */
uint16_t CRC16(const char* buf, int len);

/**
 * @brief UUID generate a new UUID string
 *
 * @return std::string UUID
 */
std::string UUID();

/**
 * @brief RandomNumber generate random number
 *
 * @param max the max random value
 * @return uint32_t
 */
uint32_t RandomNumber(uint32_t max);

/**
 * @brief RandomString generate random string
 *
 * @param length the random string length
 *
 * @return std::string random string
 */
std::string RandomString(uint32_t length);

} // namespace assist
} // namespace viper

#endif
