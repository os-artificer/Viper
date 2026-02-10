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

#ifndef _VIPER_CORE_ASSIST_MEMORY_H_
#define _VIPER_CORE_ASSIST_MEMORY_H_

namespace viper {
namespace assist {

/**
 * @brief ZeroMemory clean memory and set zero value
 *
 * @param data the memory buffer pointer
 * @param size the memory buffer size
 */
void ZeroMemory(void* data, int size);

/**
 * @brief SetMemory set memory with value
 *
 * @param data the memory buffer pointer
 * @param value the memory value
 * @param size the memory buffer size
 */
void SetMemory(void* data, char value, int size);

} // namespace assist
} // namespace viper

#endif
