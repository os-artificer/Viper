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

#ifndef _VIPER_CORE_ASSIST_EXECUTION_QUEUE_H_
#define _VIPER_CORE_ASSIST_EXECUTION_QUEUE_H_

#include <concurrentqueue/blockingconcurrentqueue.h>

#include <atomic>
#include <cstddef>
#include <functional>
#include <future>
#include <memory>
#include <string>
#include <vector>

// clang-format off

#define DEQUEUE_BULK_SIZE    4096
#define DEQUEUE_TIMEOUT_USEC 50000

// clang-format on

namespace viper {
namespace assist {

class ExecutionQueue final
{
public:
    using TaskFunctor = std::function<void()>;

public:
    ExecutionQueue(const std::string& inName, std::size_t inMaxCount);
    ExecutionQueue(const std::string& inName, std::size_t inMaxCount, std::size_t inMaxConsumerCount);
    ~ExecutionQueue();

public:
    std::error_code Enqueue(const TaskFunctor& task, std::size_t maxRetryTimes = 3);
    void            BlockEnqueue(const TaskFunctor& task);

private:
    void Consumer();

private:
    std::atomic_bool                                 _needStop = false;
    std::string                                      _name;
    std::size_t                                      _maxTaskCount = 0;
    moodycamel::BlockingConcurrentQueue<TaskFunctor> _tasks;
    std::size_t                                      _maxConsumerCount = 3;
    std::vector<std::future<void>>                   _consumers;
};

using ExecutionQueuePtr = std::shared_ptr<ExecutionQueue>;

} // namespace assist
} // namespace viper

#endif
