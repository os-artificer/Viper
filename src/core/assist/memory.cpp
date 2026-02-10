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

#include "core/assist/memory.h"

#include <cstring>

namespace viper {
namespace assist {

void ZeroMemory(void* data, int size)
{
    if (!data)
    {
        return;
    }

    std::memset(data, '\0', size);
}

void SetMemory(void* data, char value, int size)
{
    if (!data)
    {
        return;
    }

    std::memset(data, value, size);
}

} // namespace assist
} // namespace viper
