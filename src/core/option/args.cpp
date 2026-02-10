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

#include "core/option/args.h"
#include "core/text/strings.h"

#include <cstdlib>
#include <string>

namespace viper::option {

Args::Args(const std::map<std::string, std::shared_ptr<Value>>& vars)
{
    _vars = vars;
}

void Args::Set(const std::string& name, std::shared_ptr<Value> val)
{
    _vars[name] = val;
}

void Args::Set(const std::string& name, const Value& val)
{
    _vars[name] = std::make_shared<Value>(val);
}

void Args::Set(const std::string& name, int val)
{
    _vars[name] = std::make_shared<Value>(val);
}

void Args::Set(const std::string& name, float val)
{
    _vars[name] = std::make_shared<Value>(val);
}

void Args::Set(const std::string& name, double val)
{
    _vars[name] = std::make_shared<Value>(val);
}

void Args::Set(const std::string& name, bool val)
{
    _vars[name] = std::make_shared<Value>(val);
}

void Args::Set(const std::string& name, const std::string& val)
{
    _vars[name] = std::make_shared<Value>(val);
}

bool Args::Exist(const std::string& name)
{
    auto it = _vars.find(name);
    return it != _vars.end();
}

int Args::Count() const
{
    return _vars.size();
}

std::shared_ptr<Value> Args::Get(const std::string& name) const
{
    auto it = _vars.find(name);
    if (it != _vars.end())
    {
        return it->second;
    }

    return nullptr;
}

} // namespace viper::option
