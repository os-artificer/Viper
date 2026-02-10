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

#ifndef _VIPER_CORE_ASSIST_DEFER_H_
#define _VIPER_CORE_ASSIST_DEFER_H_

#include "core/assist/macro.h"

#include <functional>

#define DEFER(code) viper::assist::Defer ANONYMOUS_VAR(L, __LINE__)([&]() { code; });

namespace viper {
namespace assist {

class Defer final
{
public:
    Defer(std::function<void()> &&func)
        : _functor(std::move(func)){};

    ~Defer()
    {
        if (_functor)
        {
            _functor();
        }
    };

private:
    std::function<void()> _functor;
};

template <typename T>
class DeferAction final
{
public:
    DeferAction(T f)
        : _cleanFunctor(f){};

    ~DeferAction()
    {
        if (_cleanFunctor)
        {
            _cleanFunctor();
        }
    };

private:
    T _cleanFunctor;
};

template <typename T>
DeferAction<T> Finally(T t)
{
    return DeferAction<T>(t);
};

} // namespace assist
} // namespace viper

#endif
