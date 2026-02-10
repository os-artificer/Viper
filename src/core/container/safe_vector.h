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

#ifndef _VIPER_CORE_CONTAINER_SAFE_VECTOR_H_
#define _VIPER_CORE_CONTAINER_SAFE_VECTOR_H_

#include <algorithm>
#include <cstddef>
#include <functional>
#include <iterator>
#include <mutex>
#include <shared_mutex>
#include <vector>

namespace viper {
namespace container {

template <typename T>
class SafeVector final
{
public:
    /**
     * @brief Count return the element count in this vector
     *
     * @return std::size_t
     */
    std::size_t Count()
    {
        std::shared_lock<std::shared_mutex> lock(_mutex);
        return _elements.size();
    }

    /**
     * @brief CopyTo copy this vector to copiedElements
     *
     * @param copiedElements the output value
     */
    void CopyTo(std::vector<T>& copiedElements)
    {
        std::shared_lock<std::shared_mutex> lock(_mutex);
        std::copy(_elements.begin(), _elements.end(), std::back_inserter(copiedElements));
    }

    /**
     * @brief Clean clean this vector _elements
     *
     */
    void Clean()
    {
        std::unique_lock<std::shared_mutex> lock(_mutex);
        _elements.clear();
    }

    /**
     * @brief At get the element by the index, if the index is over the element count, will return zero value.
     *
     * @param index the element index in this vector
     * @return T
     */
    T At(std::size_t index)
    {
        std::shared_lock<std::shared_mutex> lock(_mutex);

        if (_elements.size() <= index)
        {
            return T();
        }

        return _elements.at(index);
    }

    /**
     * @brief Delete delete the element which equal with the value
     *
     * @param value the value need to delete
     */
    void Delete(T value)
    {
        std::unique_lock<std::shared_mutex> lock(_mutex);

        for (auto eleIter = _elements.begin(); eleIter != _elements.end();)
        {
            if (*eleIter == value)
            {
                eleIter = _elements.erase(eleIter);
            }
            else
            {
                ++eleIter;
            }
        }
    }

    /**
     * @brief Delete delete the value when the needDelete return true
     *
     * @param needDelete the function used to check the value, if it need to be deleted.
     */
    void Delete(std::function<bool(T)> needDelete)
    {
        std::unique_lock<std::shared_mutex> lock(_mutex);

        for (auto eleIter = _elements.begin(); eleIter != _elements.end();)
        {
            if (needDelete(*eleIter))
            {
                eleIter = _elements.erase(eleIter);
            }
            else
            {
                ++eleIter;
            }
        }
    }

    /**
     * @brief Swap exchanges the _elements between two vectors
     *
     * @param swappedElements the _elements will be exchanged
     */
    void Swap(std::vector<T>& swappedElements)
    {
        std::unique_lock<std::shared_mutex> lock(_mutex);
        _elements.swap(swappedElements);
    }

    /**
     * @brief Exists checks if the element exists
     *
     * @param element the element in this vector
     * @return true found the element in this vector
     * @return false not found the element in this vector
     */
    bool Exists(const T& element)
    {
        std::shared_lock<std::shared_mutex> lock(_mutex);

        auto eleIter = std::find(_elements.begin(), _elements.end(), element);
        return eleIter != _elements.end();
    }

    /**
     * @brief Find find the element in this vector
     *
     * @param element fo find in this vector
     * @return T
     */
    T Find(const T& element)
    {
        std::shared_lock<std::shared_mutex> lock(_mutex);

        auto eleIter = std::find(_elements.begin(), _elements.end(), element);
        if (eleIter != _elements.end())
        {
            return *eleIter;
        }

        return T();
    }

    void Push(const T& element)
    {
        std::unique_lock<std::shared_mutex> lock(_mutex);
        _elements.push_back(element);
    }

private:
    std::shared_mutex _mutex;
    std::vector<T>    _elements;
};

} // namespace container
} // namespace viper

#endif
