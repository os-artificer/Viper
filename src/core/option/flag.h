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

#ifndef _VIPER_OPTION_FLAG_H_
#define _VIPER_OPTION_FLAG_H_

#include <string>

namespace viper::option {

/**
 * @brief Template representing a single command-line flag/option.
 *
 * Instances of `Flag<T>` describe a command option (long name, optional
 * shorthand, usage description and default/current value). The template
 * parameter `T` is the stored type; use `Flag<Value>` when you need
 * runtime polymorphism (heterogeneous types).
 *
 * @tparam T The C++ type stored for this flag (int, bool, std::string, Value, ...)
 */
template <typename T>
class Flag final
{
public:
    /**
     * @brief Constructs a flag with only a name.
     * @param name The long name of the flag (e.g., "verbose")
     */
    Flag(const std::string& name)
        : _name(name) {}

    /**
     * @brief Constructs a flag with name and shorthand.
     * @param name The long name of the flag
     * @param shorthand The one-letter abbreviation (e.g., "v")
     */
    Flag(const std::string& name, const std::string& shorthand)
        : _name(name), _shorthand(shorthand) {}

    /**
     * @brief Constructs a flag with name, shorthand, and usage description.
     * @param name The long name of the flag
     * @param shorthand The one-letter abbreviation
     * @param usage Help message describing the flag
     */
    Flag(const std::string& name, const std::string& shorthand, const std::string& usage)
        : _name(name), _shorthand(shorthand), _usage(usage) {}

    /**
     * @brief Constructs a flag with all parameters including default value.
     * @param name The long name of the flag
     * @param shorthand The one-letter abbreviation
     * @param usage Help message describing the flag
     * @param value The default value for this flag
     */
    Flag(const std::string& name, const std::string& shorthand, const std::string& usage, const T& value)
        : _name(name), _shorthand(shorthand), _usage(usage), _value(value) {}

public:
    /// @brief The long form flag name as it appears on command line (e.g., "verbose", "output-file")
    std::string _name;

    /// @brief The one-letter abbreviated flag for convenience (e.g., "v", "o")
    std::string _shorthand;

    /// @brief The help/usage message that describes what this flag does
    std::string _usage;

    /// The textual type name (optional, used for help output / reflection).
    std::string _valueType;

    /// The current value (or default) for this flag.
    T _value;
};

} // namespace viper::option

#endif
