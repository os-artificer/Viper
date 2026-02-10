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

#ifndef _VIPER_CORE_ASSIST_EXECUTION_MULTI_QUEUE_H_
#define _VIPER_CORE_ASSIST_EXECUTION_MULTI_QUEUE_H_

#include "core/assist/execution_queue.h"

namespace viper {
namespace assist {

class ExecutionMultiQueue final
{
public:
    ExecutionMultiQueue(const std::string& inName, std::size_t inQueueSize, std::size_t inQueueCount);
    ExecutionMultiQueue(const std::string& inName, std::size_t inQueueSize, std::size_t inQueueCount, std::size_t inConsumerCount);
    ~ExecutionMultiQueue();

public:
    std::error_code Enqueue(const ExecutionQueue::TaskFunctor& task, std::size_t maxRetryTimes = 3);
    std::error_code Enqueue(uint32_t hashCode, const ExecutionQueue::TaskFunctor& task, std::size_t maxRetryTimes = 3);
    void            BlockEnqueue(const ExecutionQueue::TaskFunctor& task);
    void            BlockEnqueue(uint32_t hashCode, const ExecutionQueue::TaskFunctor& task);

private:
    std::string                    _name;
    std::size_t                    _queueSize     = 0;
    std::size_t                    _queueCount    = 0;
    std::size_t                    _consumerCount = 3;
    std::vector<ExecutionQueuePtr> _queues;
};

using ExecutionMultiQueuePtr = std::shared_ptr<ExecutionMultiQueue>;

} // namespace assist
} // namespace viper

#endif
