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

#include "core/assist/time.h"
#include "core/assist/memory.h"

#include <chrono>
#include <cstdlib>

#include <unistd.h>

namespace viper {
namespace assist {

void Sleep(uint32_t secs)
{
    sleep(secs);
}

void MilliSleep(uint32_t milliSecs)
{
    usleep(milliSecs * 1000);
}

int Timezone()
{
    std::time_t now     = std::time(nullptr);
    std::tm     localTM = {0};

    char timezone[10];
    ZeroMemory(timezone, sizeof(timezone));

    localtime_r(&now, &localTM);
    std::strftime(timezone, sizeof(timezone), "%z", &localTM);

    return std::atoi(timezone) / 100;
}

uint32_t TimestampSecond()
{
    std::chrono::system_clock::time_point now     = std::chrono::system_clock::now();
    std::chrono::seconds                  seconds = std::chrono::duration_cast<std::chrono::seconds>(now.time_since_epoch());
    return seconds.count();
}

uint64_t TimestampMillisecond()
{
    std::chrono::system_clock::time_point now          = std::chrono::system_clock::now();
    std::chrono::milliseconds             milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch());
    return milliseconds.count();
}

uint64_t TimestampMicrosecond()
{
    std::chrono::system_clock::time_point now          = std::chrono::system_clock::now();
    std::chrono::microseconds             microseconds = std::chrono::duration_cast<std::chrono::microseconds>(now.time_since_epoch());
    return microseconds.count();
}

uint64_t TimestampTickCountSecond()
{
    std::chrono::steady_clock::time_point now     = std::chrono::steady_clock::now();
    std::chrono::seconds                  seconds = std::chrono::duration_cast<std::chrono::seconds>(now.time_since_epoch());
    return seconds.count();
}

uint64_t TimestampTickCountMillisecond()
{
    std::chrono::steady_clock::time_point now          = std::chrono::steady_clock::now();
    std::chrono::milliseconds             milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch());
    return milliseconds.count();
}

uint64_t TimestampTickCountMicrosecond()
{
    std::chrono::steady_clock::time_point now          = std::chrono::steady_clock::now();
    std::chrono::microseconds             microseconds = std::chrono::duration_cast<std::chrono::microseconds>(now.time_since_epoch());
    return microseconds.count();
}

std::string TimeToString(std::time_t time)
{
    return TimeToString(time, "%Y-%m-%d %H:%M:%S");
}

std::string TimeToString(std::time_t time, const std::string& formatter)
{
    std::tm localTM = {0};

    localtime_r(&time, &localTM);

    char formatedTime[24];
    ZeroMemory(formatedTime, sizeof(formatedTime));

    std::strftime(formatedTime, sizeof(formatedTime), formatter.c_str(), &localTM);
    return std::string(formatedTime);
}

} // namespace assist
} // namespace viper
