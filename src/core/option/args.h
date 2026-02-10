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

#ifndef _VIPER_OPTION_ARG_H_
#define _VIPER_OPTION_ARG_H_

#include "core/option/value.h"

#include <map>
#include <memory>
#include <string>

namespace viper::option {

/// Container for parsed command-line arguments.
///
/// `Args` holds a map of argument name -> `Value` and is used to pass
/// parsed values into command callbacks (`Command::_run`). Values are
/// stored as `shared_ptr<Value>` so ownership can be shared between
/// commands and client code.
class Args final
{
public:
    /// Default construct an empty `Args` container.
    Args() = default;

    /// Construct from an existing name->Value map.
    /// @param vars Pre-populated argument map (shared_ptr ownership).
    Args(const std::map<std::string, std::shared_ptr<Value>>&vars);

public:
    /// Set a value by supplying a `shared_ptr<Value>`.
    void Set(const std::string& name, std::shared_ptr<Value> val);

    /// Set a value by copying a `Value` instance.
    void Set(const std::string& name, const Value& val);

    /// Convenience overloads for common primitive types.
    void Set(const std::string& name, int val);
    void Set(const std::string& name, float val);
    void Set(const std::string& name, double val);
    void Set(const std::string& name, bool val);
    void Set(const std::string& name, const std::string& val);

    /// Check whether an argument with `name` exists.
    /// @return true if present, false otherwise.
    bool Exist(const std::string& name);

    /// Number of stored arguments.
    int Count() const;

    /// Retrieve the Value for `name`.
    /// @return shared_ptr<Value> or nullptr if not found.
    std::shared_ptr<Value> Get(const std::string& name) const;

private:
    /// Internal storage for parsed arguments: name -> value.
    std::map<std::string, std::shared_ptr<Value>> _vars;
};

} // namespace viper::option

#endif
