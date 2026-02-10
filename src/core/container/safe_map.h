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

#ifndef _VIPER_CORE_CONTAINER_SAFE_MAP_H_
#define _VIPER_CORE_CONTAINER_SAFE_MAP_H_

#include <algorithm>
#include <cstddef>
#include <functional>
#include <iterator>
#include <map>
#include <mutex>
#include <shared_mutex>

namespace viper {
namespace container {

template <typename KEY, typename VAL>
class SafeMap final
{
public:
    /**
     * @brief Swap swap with swappedElements
     *
     * @param swappedElements the swap value is swapped
     */
    void Swap(std::map<KEY, VAL>& swappedElements)
    {
        std::unique_lock<std::shared_mutex> lock(_mutex);

        _elements.swap(swappedElements);
    }

    /**
     * @brief Count return the element count of this map
     *
     * @return std::size_t
     */
    std::size_t Count()
    {
        std::shared_lock<std::shared_mutex> lock(_mutex);
        return _elements.size();
    }

    /**
     * @brief CopyTo copy values to copiedElements
     *
     * @param copiedElements the output value
     */
    void CopyTo(std::map<KEY, VAL>& copiedElements)
    {
        std::shared_lock<std::shared_mutex> lock(_mutex);

        std::copy(_elements.begin(), _elements.end(), std::inserter(copiedElements, copiedElements.end()));
    }

    /**
     * @brief Push insert the key,value into map, if the key is already exists, the exists key will be replaced.
     *
     * @param key the map key
     * @param value the map value
     * @return std::size_t the storaged element count
     */
    std::size_t Push(KEY key, VAL value)
    {
        std::unique_lock<std::shared_mutex> lock(_mutex);

        _elements[key] = value;

        return _elements.size();
    }

    /**
     * @brief Exists check the key if it's exists
     *
     * @param key the map key
     * @return true the key is exists in the map
     * @return false the key is not exists in the map
     */
    bool Exists(KEY key)
    {
        std::shared_lock<std::shared_mutex> lock(_mutex);

        return _elements.find(key) != _elements.end();
    }

    /**
     * @brief Exists check the (key,value) if it's exists
     *
     * @param key the map key
     * @param value the map value
     * @return true the (key,value) is exists in the map
     * @return false the (key, value) is not exists in the map
     */
    bool Exists(KEY key, VAL value)
    {
        std::shared_lock<std::shared_mutex> lock(_mutex);

        auto iter = _elements.find(key);

        if (iter == _elements.end())
        {
            return false;
        }

        return iter->second == value;
    }

    VAL Remove(KEY key)
    {
        std::unique_lock<std::shared_mutex> lock(_mutex);

        auto iter = _elements.find(key);
        if (iter == _elements.end())
        {
            return nullptr;
        }

        return iter->second;
    }

    /**
     * @brief Delete delete the key from the map
     *
     * @param key the map key
     */
    void Delete(KEY key)
    {
        std::unique_lock<std::shared_mutex> lock(_mutex);
        _elements.erase(key);
    }

    /**
     * @brief DeleteByValue delete the all keys from the map which has the value
     *
     * @param value the map value
     */
    void DeleteByValue(VAL value)
    {
        std::unique_lock<std::shared_mutex> lock(_mutex);

        for (auto eleIter = _elements.begin(); eleIter != _elements.end();)
        {

            if (eleIter->second == value)
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
     * @brief Delete delete the (key,value) when the needDelete return true
     *
     * @param needDelete the function used to check the (key,value) if it need to be deleted.
     */
    void Delete(std::function<bool(KEY key, VAL value)> needDelete)
    {
        std::unique_lock<std::shared_mutex> lock(_mutex);

        for (auto eleIter = _elements.begin(); eleIter != _elements.end();)
        {

            if (needDelete(eleIter->first, eleIter->second))
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
     * @brief Find get the value by the key
     *
     * @param key the map key
     * @param value the map value
     * @return true found the value by the key
     * @return false not found the value by the key
     */
    bool Find(KEY key, VAL& value)
    {
        std::shared_lock<std::shared_mutex> lock(_mutex);

        auto eleIter = _elements.find(key);
        if (eleIter == _elements.end())
        {
            return false;
        }

        value = eleIter->second;
        return true;
    }

    /**
     * @brief Find find the value by the key
     *
     * @param key the map key
     * @return VAL if do not find the key, return the default VAL
     */
    VAL Find(const KEY& key)
    {
        std::shared_lock<std::shared_mutex> lock(_mutex);

        auto eleIter = _elements.find(key);
        if (eleIter == _elements.end())
        {
            return VAL();
        }

        return eleIter->second;
    }

    /**
     * @brief Clean clean all _elements from this map
     *
     */
    void Clean()
    {
        std::unique_lock<std::shared_mutex> lock(_mutex);
        _elements.clear();
    }

    /**
     * @brief Foreach call callback function for each element in this map
     *
     * @param callback
     */
    void Foreach(std::function<void(KEY, VAL)> callback)
    {
        std::unique_lock<std::shared_mutex> lock(_mutex);

        for (auto ele : _elements)
        {
            callback(ele.first, ele.second);
        }
    }

    /**
     * @brief ForeachAndDoOnce call callback function for each element in this map, if do success once, break loop.
     *
     * @param callback
     */
    void ForeachAndDoOnce(std::function<bool(KEY, VAL)> callback)
    {
        std::unique_lock<std::shared_mutex> lock(_mutex);

        for (auto ele : _elements)
        {
            if (callback(ele.first, ele.second))
            {
                break;
            }
        }
    }

private:
    std::shared_mutex  _mutex;
    std::map<KEY, VAL> _elements;
};

} // namespace container
} // namespace viper

#endif
