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

#include "core/assist/host.h"
#include "core/assist/memory.h"
#include "core/error/error.h"
#include "core/log/log.h"

#include <cstring>
#include <fstream>
#include <sstream>
#include <string>
#include <system_error>

#include <pwd.h>
#include <unistd.h>

namespace viper {
namespace assist {

int HostCPUCount()
{
    return std::thread::hardware_concurrency();
}

double HostCPUFrequency()
{
    int    cnt       = 0;
    double frequency = 0.0;

    std::ifstream cpuinfo("/proc/cpuinfo");
    std::string   line;

    if (!cpuinfo.is_open())
    {
        return -1.0;
    }

    while (std::getline(cpuinfo, line))
    {
        if (line.find("cpu MHz") != std::string::npos)
        {
            double             freq = 0.0;
            std::istringstream iss(line);
            std::string        key;
            std::getline(iss, key, ':');
            iss >> freq;
            frequency += freq;
            ++cnt;
        }
    }

    cpuinfo.close();

    return cnt > 0 ? (frequency / cnt) : frequency;
}

double HostCPUUsage()
{
    return 0.0;
}

std::error_code Hostname(std::string& hostName)
{
    char buffer[128];
    ZeroMemory(buffer, sizeof(buffer));

    if (gethostname(buffer, sizeof(buffer) - 1) < 0)
    {
        LOG_ERROR("can not get host name. errmsg({})", error::ToString(errno));
        return error::ErrorCode::ERROR;
    }

    if (strlen(buffer) > 0 && strlen(buffer) < sizeof(buffer))
    {
        hostName = buffer;
        return error::ErrorCode::SUCCESS;
    }

    return error::ErrorCode::ERROR;
}

} // namespace assist
} // namespace viper

