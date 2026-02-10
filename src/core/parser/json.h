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

#ifndef _VIPER_CORE_PARSER_JSON_H_
#define _VIPER_CORE_PARSER_JSON_H_

#include <rapidjson/document.h>
#include <rapidjson/error/en.h>
#include <rapidjson/error/error.h>
#include <rapidjson/prettywriter.h>
#include <rapidjson/rapidjson.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

#include <string>
#include <vector>

namespace rj = rapidjson;

namespace viper {
namespace parser {

inline void SetErrMsg(std::string* msg, const std::string& errMsg)
{
    if (!msg)
    {
        *msg = errMsg;
    }
}

template <typename T>
std::string JToString(const T& t, bool isPretty = false)
{
    rj::StringBuffer buffer;

    if (isPretty)
    {
        rj::PrettyWriter<rj::StringBuffer> writer(buffer);
        t.Accept(writer);
    }
    else
    {
        rj::Writer<rj::StringBuffer> writer(buffer);
        t.Accept(writer);
    }

    return buffer.GetString();
}

inline bool JLoad(const std::string& json, rj::Document& doc, std::string* errMsg)
{
    rj::ParseResult ok = doc.Parse(json);

    if (!ok)
    {
        SetErrMsg(errMsg, rj::GetParseError_En(ok.Code()));
        return false;
    }

    return true;
}

template <typename VAL>
struct JResult final
{
    bool        _ok      = true;
    VAL         _value   = VAL();
    std::string _message = {""};

    explicit operator bool() const
    {
        return _ok;
    }
};

template <typename VAL, typename T = VAL>
struct JValue final
{
};

template <typename VAL>
struct JValue<VAL, std::string> final
{
    static void Get(const rj::Value::ConstMemberIterator& iter, JResult<VAL>& result)
    {
        if (iter->value.IsString())
        {
            result._value = iter->value.GetString();
        }
        else
        {
            result._ok      = false;
            result._message = "invalid value type";
        }
    }
};

template <typename VAL>
struct JValue<VAL, int> final
{
    static void Get(const rj::Value::ConstMemberIterator& iter, JResult<VAL>& result)
    {
        if (iter->value.IsInt())
        {
            result._value = iter->value.GetInt();
        }
        else
        {
            result._ok      = false;
            result._message = "invalid value type";
        }
    }
};

template <typename VAL>
struct JValue<VAL, uint64_t> final
{
    static void Get(const rj::Value::ConstMemberIterator& iter, JResult<VAL>& result)
    {
        if (iter->value.IsUint64())
        {
            result._value = iter->value.GetUint64();
        }
        else
        {
            result._ok      = false;
            result._message = "invalid value type";
        }
    }
};

template <typename VAL>
struct JValue<VAL, int64_t> final
{
    static void Get(const rj::Value::ConstMemberIterator& iter, JResult<VAL>& result)
    {
        if (iter->value.IsInt64())
        {
            result._value = iter->value.GetInt64();
        }
        else
        {
            result._ok      = false;
            result._message = "invalid value type";
        }
    }
};

template <typename VAL>
struct JValue<VAL, bool> final
{
    static void Get(const rj::Value::ConstMemberIterator& iter, JResult<VAL>& result)
    {
        if (iter->value.IsBool())
        {
            result._value = iter->value.GetBool();
        }
        else
        {
            result._ok      = false;
            result._message = "invalid value type";
        }
    }
};

template <typename VAL>
struct JValue<VAL, float> final
{
    static void Get(const rj::Value::ConstMemberIterator& iter, JResult<VAL>& result)
    {
        if (iter->value.IsFloat())
        {
            result._value = iter->value.GetFloat();
        }
        else
        {
            result._ok      = false;
            result._message = "invalid value type";
        }
    }
};

template <typename VAL>
struct JValue<VAL, double> final
{
    static void Get(const rj::Value::ConstMemberIterator& iter, JResult<VAL>& result)
    {
        if (iter->value.IsDouble())
        {
            result._value = iter->value.GetDouble();
        }
        else
        {
            result._ok      = false;
            result._message = "invalid value type";
        }
    }
};

template <typename VAL>
struct JValue<VAL, std::vector<int>> final
{
    static void Get(const rj::Value::ConstMemberIterator& iter, JResult<VAL>& result)
    {
        if (iter->value.IsArray())
        {
            for (auto& val : iter->value.GetArray())
            {
                if (val.IsInt())
                {
                    result._value.push_back(val.GetInt());
                }
            }
        }
        else
        {
            result._ok      = false;
            result._message = "invalid value type";
        }
    }
};

template <typename VAL>
struct JValue<VAL, std::vector<int64_t>> final
{
    static void Get(const rj::Value::ConstMemberIterator& iter, JResult<VAL>& result)
    {
        if (iter->value.IsArray())
        {
            for (auto& val : iter->value.GetArray())
            {
                if (val.IsInt64())
                {
                    result._value.push_back(val.GetInt64());
                }
            }
        }
        else
        {
            result._ok      = false;
            result._message = "invalid value type";
        }
    }
};

template <typename VAL>
struct JValue<VAL, std::vector<uint64_t>> final
{
    static void Get(const rj::Value::ConstMemberIterator& iter, JResult<VAL>& result)
    {
        if (iter->value.IsArray())
        {
            for (auto& val : iter->value.GetArray())
            {
                if (val.IsUint64())
                {
                    result._value.push_back(val.GetUint64());
                }
            }
        }
        else
        {
            result._ok      = false;
            result._message = "invalid value type";
        }
    }
};

template <typename VAL>
struct JValue<VAL, std::vector<double>> final
{
    static void Get(const rj::Value::ConstMemberIterator& iter, JResult<VAL>& result)
    {
        if (iter->value.IsArray())
        {
            for (auto& val : iter->value.GetArray())
            {
                if (val.IsDouble())
                {
                    result._value.push_back(val.GetDouble());
                }
            }
        }
        else
        {
            result._ok      = false;
            result._message = "invalid value type";
        }
    }
};

template <typename VAL>
struct JValue<VAL, std::vector<std::string>> final
{
    static void Get(const rj::Value::ConstMemberIterator& iter, JResult<VAL>& result)
    {
        if (iter->value.IsArray())
        {
            for (auto& val : iter->value.GetArray())
            {
                if (val.IsString())
                {
                    result._value.push_back(val.GetString());
                }
            }
        }
        else
        {
            result._ok      = false;
            result._message = "invalid value type";
        }
    }
};

template <typename VAL>
struct JHelper final
{
    static JResult<VAL> Get(const rj::Document& doc, const char* name, const VAL& defaultVal = VAL())
    {
        JResult<VAL> result;
        result._value = defaultVal;

        rj::Value::ConstMemberIterator iter = doc.FindMember(name);
        if (iter == doc.MemberEnd())
        {
            result._ok      = false;
            result._message = "invalid key '" + std::string(name) + "'";
            return result;
        }

        JValue<VAL>::Get(iter, result);
        return result;
    }

    static JResult<VAL> Get(const rj::Value& val, const char* name, const VAL& defaultVal = VAL())
    {
        JResult<VAL> result;
        result._value = defaultVal;

        rj::Value::ConstMemberIterator iter = val.FindMember(name);
        if (iter == val.MemberEnd())
        {
            result._ok      = false;
            result._message = "invalid key '" + std::string(name) + "'";
            return result;
        }

        JValue<VAL>::Get(iter, result);
        return result;
    }

    static JResult<VAL> Get(const rj::Document& doc, const std::vector<std::string>& names, const VAL& defaultVal = VAL())
    {
        JResult<VAL> result;
        result._value = defaultVal;

        for (auto& name : names)
        {
            rj::Value::ConstMemberIterator iter = doc.FindMember(name);
            if (iter == doc.MemberEnd())
            {
                continue;
            }

            JValue<VAL>::Get(iter, result);
            return result;
        }

        result._ok      = false;
        result._message = "invalid keys";
        return result;
    }

    static JResult<VAL> Get(const rj::Value& val, const std::vector<std::string>& names, const VAL& defaultVal = VAL())
    {
        JResult<VAL> result;
        result._value = defaultVal;

        for (auto& name : names)
        {
            rj::Value::ConstMemberIterator iter = val.FindMember(name);
            if (iter == val.MemberEnd())
            {
                continue;
            }

            JValue<VAL>::Get(iter, result);
            return result;
        }

        result._ok      = false;
        result._message = "invalid keys";
        return result;
    }
};

} // namespace parser
} // namespace viper

#endif

