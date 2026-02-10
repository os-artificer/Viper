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

#include "core/assist/execution_multi_queue.h"
#include "core/assist/execution_queue.h"
#include "core/assist/string.h"

#include <atomic>
#include <memory>

namespace viper {
namespace assist {

ExecutionMultiQueue::ExecutionMultiQueue(const std::string& inName, std::size_t inQueueSize, std::size_t inQueueCount)
{
    _name       = inName;
    _queueSize  = inQueueSize;
    _queueCount = inQueueCount;

    for (auto idx = 0; idx < inQueueCount; ++idx)
    {
        auto qname = FormatString("%s-%d", _name.c_str(), idx);
        _queues.push_back(std::make_shared<ExecutionQueue>(qname, _queueSize));
    }
}

ExecutionMultiQueue::ExecutionMultiQueue(const std::string& inName, std::size_t inQueueSize, std::size_t inQueueCount, std::size_t inConsumerCount)
{
    _name          = inName;
    _queueSize     = inQueueSize;
    _queueCount    = inQueueCount;
    _consumerCount = inConsumerCount;

    for (auto idx = 0; idx < _queueCount; ++idx)
    {
        auto qname = FormatString("%s-%d", _name.c_str(), idx);
        _queues.push_back(std::make_shared<ExecutionQueue>(qname, _queueSize));
    }
}

ExecutionMultiQueue::~ExecutionMultiQueue()
{
    _queues.clear();
}

std::error_code ExecutionMultiQueue::Enqueue(const ExecutionQueue::TaskFunctor& task, std::size_t maxRetryTimes)
{

    static std::atomic_uint64_t idx = 0;

    auto pos = ++idx % _queueCount;
    return _queues[pos]->Enqueue(task, maxRetryTimes);
}

std::error_code ExecutionMultiQueue::Enqueue(uint32_t hashCode, const ExecutionQueue::TaskFunctor& task, std::size_t maxRetryTimes)
{
    auto pos = hashCode % _queueCount;
    return _queues[pos]->Enqueue(task, maxRetryTimes);
}

void ExecutionMultiQueue::BlockEnqueue(const ExecutionQueue::TaskFunctor& task)
{
    static std::atomic_uint64_t idx = 0;

    auto pos = ++idx % _queueCount;
    _queues[pos]->BlockEnqueue(task);
}

void ExecutionMultiQueue::BlockEnqueue(uint32_t hashCode, const ExecutionQueue::TaskFunctor& task)
{
    auto pos = hashCode % _queueCount;
    _queues[pos]->BlockEnqueue(task);
}

} // namespace assist
} // namespace viper
