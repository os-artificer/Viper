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

#include "core/assist/execution_queue.h"
#include "core/assist/time.h"
#include "core/error/error.h"

#include <cstddef>

namespace viper {
namespace assist {

ExecutionQueue::ExecutionQueue(const std::string& inName, std::size_t inMaxCount)
{
    _needStop     = false;
    _name         = inName;
    _maxTaskCount = inMaxCount;

    _tasks = moodycamel::BlockingConcurrentQueue<TaskFunctor>();
    for (std::size_t idx = 0; idx < _maxConsumerCount; ++idx)
    {
        _consumers.push_back(std::async(std::launch::async, &ExecutionQueue::Consumer, this));
    }
}

ExecutionQueue::ExecutionQueue(const std::string& inName, std::size_t inMaxCount, std::size_t inMaxConsumerCount)
{
    _needStop         = false;
    _name             = inName;
    _maxTaskCount     = inMaxCount;
    _maxConsumerCount = inMaxConsumerCount;

    _tasks = moodycamel::BlockingConcurrentQueue<TaskFunctor>();
    for (std::size_t idx = 0; idx < _maxConsumerCount; ++idx)
    {
        _consumers.push_back(std::async(std::launch::async, &ExecutionQueue::Consumer, this));
    }
}

ExecutionQueue::~ExecutionQueue()
{
    _needStop = true;
    for (auto& c : _consumers)
    {
        c.wait();
    }
}

std::error_code ExecutionQueue::Enqueue(const TaskFunctor& task, std::size_t maxRetryTimes)
{
    if (_tasks.size_approx() > _maxTaskCount)
    {
        return error::ErrorCode::QUEUE_OVERFLOW;
    }

    if (!_tasks.enqueue(task))
    {
        return error::ErrorCode::ERROR;
    }

    return error::ErrorCode::SUCCESS;
}

void ExecutionQueue::BlockEnqueue(const TaskFunctor& task)
{
    while (!_tasks.enqueue(task))
    {
        MilliSleep(100);
    }
}

void ExecutionQueue::Consumer()
{
    while (!_needStop)
    {
        TaskFunctor dTasks[DEQUEUE_BULK_SIZE];

        auto cnt = _tasks.wait_dequeue_bulk_timed(dTasks, DEQUEUE_BULK_SIZE, DEQUEUE_TIMEOUT_USEC);

        for (std::size_t idx = 0; idx < cnt; ++idx)
        {
            dTasks[idx]();
        }
    }
}

} // namespace assist
} // namespace viper

