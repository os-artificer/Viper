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

#include "internal/controller.h"

#include "core/parser/json.h"

namespace viper {
namespace internal {

std::string ServiceInfo::ToJSON()
{
    rj::Document doc(rj::kObjectType);

    auto& allocator = doc.GetAllocator();

    doc.AddMember("service_id", _serviceID, allocator);
    doc.AddMember("service_name", _serviceName, allocator);
    doc.AddMember("listen_address", _listeningAddress, allocator);
    doc.AddMember("api_address", _apiAddress, allocator);
    doc.AddMember("load", _load, allocator);
    doc.AddMember("updated_timestamp", _updatedTimestamp, allocator);

    return viper::parser::JToString(doc);
}

bool ServiceInfo::LoadJSON(const std::string& json, std::string& errMsg)
{
    rj::Document doc;
    if (!viper::parser::JLoad(json, doc, &errMsg))
    {
        return false;
    }

    auto serviceID = viper::parser::JHelper<std::string>::Get(doc, "service_id");
    if (serviceID._ok)
    {
        _serviceID = serviceID._value;
    }

    auto serviceName = viper::parser::JHelper<std::string>::Get(doc, "service_name");
    if (serviceName._ok)
    {
        _serviceName = serviceName._value;
    }

    auto listenAddress = viper::parser::JHelper<std::string>::Get(doc, "listen_address");
    if (listenAddress._ok)
    {
        _listeningAddress = listenAddress._value;
    }

    auto apiAddress = viper::parser::JHelper<std::string>::Get(doc, "api_address");
    if (apiAddress._ok)
    {
        _apiAddress = apiAddress._value;
    }

    auto load = viper::parser::JHelper<float>::Get(doc, "load");
    if (load._ok)
    {
        _load = load._value;
    }

    auto updatedTimestamp = viper::parser::JHelper<uint64_t>::Get(doc, "updated_timestamp");
    if (updatedTimestamp._ok)
    {
        _updatedTimestamp = updatedTimestamp._value;
    }

    return true;
}

} // namespace internal
} // namespace viper
