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

#ifndef _VIPER_INTERNAL_CONTROLLER_H_
#define _VIPER_INTERNAL_CONTROLLER_H_

#include "core/net/message.h"

#include "internal/controller.pb.h"

#include <cstdint>

namespace viper {
namespace internal {

#define PROTOCOL_VERSION_01 0x01

enum class MessageType : uint32_t
{
    // CONTROLLER: VIPER_NET_MESSAGE_PROTOCOL_BASE + [0x001,0x100)
    PING                       = (VIPER_NET_MESSAGE_PROTOCOL_BASE + 0x001),
    PONG                       = (VIPER_NET_MESSAGE_PROTOCOL_BASE + 0x002),
    AUTH_REQUEST               = (VIPER_NET_MESSAGE_PROTOCOL_BASE + 0x003),
    AUTH_RESPONSE              = (VIPER_NET_MESSAGE_PROTOCOL_BASE + 0x004),
    REGISTER_AGENT_REQUEST     = (VIPER_NET_MESSAGE_PROTOCOL_BASE + 0x005),
    REGISTER_AGENT_RESPONSE    = (VIPER_NET_MESSAGE_PROTOCOL_BASE + 0x006),
    UNREGISTER_AGENT_REQUEST   = (VIPER_NET_MESSAGE_PROTOCOL_BASE + 0x007),
    UNREGISTER_AGENT_RESPONSE  = (VIPER_NET_MESSAGE_PROTOCOL_BASE + 0x008),
    REPORT_AGENT_STATE         = (VIPER_NET_MESSAGE_PROTOCOL_BASE + 0x009),
    REGISTER_PLUGIN_REQUEST    = (VIPER_NET_MESSAGE_PROTOCOL_BASE + 0x00A),
    REGISTER_PLUGIN_RESPONSE   = (VIPER_NET_MESSAGE_PROTOCOL_BASE + 0x00B),
    UNREGISTER_PLUGIN_REQUEST  = (VIPER_NET_MESSAGE_PROTOCOL_BASE + 0x00C),
    UNREGISTER_PLUGIN_RESPONSE = (VIPER_NET_MESSAGE_PROTOCOL_BASE + 0x00D),
    REPORT_PLUGIN_STATE        = (VIPER_NET_MESSAGE_PROTOCOL_BASE + 0x00E),
    OPERATE_PLUGIN_REQUEST     = (VIPER_NET_MESSAGE_PROTOCOL_BASE + 0x00F),
    OPERATE_PLUGIN_RESPONSE    = (VIPER_NET_MESSAGE_PROTOCOL_BASE + 0x010),

    // ROUTE: VIPER_NET_MESSAGE_PROTOCOL_BASE + [0x100, 0x200)
    ROUTE_BROADCAST_REQUEST  = (VIPER_NET_MESSAGE_PROTOCOL_BASE + 0x100),
    ROUTE_BROADCAST_RESPONSE = (VIPER_NET_MESSAGE_PROTOCOL_BASE + 0x101),
    ROUTE_SYNC_REQUEST       = (VIPER_NET_MESSAGE_PROTOCOL_BASE + 0x102),
    ROUTE_SYNC_RESPONSE      = (VIPER_NET_MESSAGE_PROTOCOL_BASE + 0x103),
};

struct ServiceInfo
{
    std::string _serviceID;
    std::string _serviceName;
    std::string _listeningAddress;
    std::string _apiAddress;

    // load value is between [0, 100]
    float    _load             = 0.0;
    uint64_t _updatedTimestamp = 0;

    std::string ToJSON();
    bool        LoadJSON(const std::string& json, std::string& errMsg);
};

} // namespace internal
} // namespace viper

#endif

