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

#ifndef _VIPER_CORE_CONTAINER_SAFE_SET_H_
#define _VIPER_CORE_CONTAINER_SAFE_SET_H_

#include <cstddef>
#include <mutex>
#include <set>
#include <shared_mutex>

namespace viper {
namespace container {

template <typename T>
class SafeSet final
{
public:
    /**
     * @brief Count return the element count of this set
     *
     * @return std::size_t
     */
    std::size_t Count()
    {
        std::shared_lock<std::shared_mutex> lock(_mutex);
        return _elements.size();
    }

    /**
     * @brief Insert insert the value into this set
     *
     * @param value the inserted value
     * @return true success
     * @return false failure
     */
    bool Insert(const T& value)
    {
        std::unique_lock<std::shared_mutex> lock(_mutex);

        auto result = _elements.insert(value);
        return result.first != _elements.end();
    }

    /**
     * @brief Empty checks whether this set is empty
     *
     * @return true this set is empty
     * @return false this set is not empty
     */
    bool Empty()
    {
        std::shared_lock<std::shared_mutex> lock(_mutex);
        return _elements.empty();
    }

    /**
     * @brief Clean clears this set
     *
     */
    void Clean()
    {
        std::unique_lock<std::shared_mutex> lock(_mutex);
        _elements.clear();
    }

    /**
     * @brief Swap swaps the contents
     *
     * @param swappedElements the new contents
     */
    void Swap(T& swappedElements)
    {
        std::unique_lock<std::shared_mutex> lock(_mutex);
        _elements.swap(swappedElements);
    }

    /**
     * @brief Exists check whether the value exists in this set
     *
     * @param value the element found in this set
     * @return true found this element in this set
     * @return false do not find this element in this set
     */
    bool Exists(const T& value)
    {
        std::shared_lock<std::shared_mutex> lock(_mutex);
        return _elements.count(value) > 0;
    }

private:
    std::shared_mutex _mutex;
    std::set<T>       _elements;
};

} // namespace container
} // namespace viper

#endif
