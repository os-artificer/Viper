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

#ifndef _VIPER_CORE_NET_MESSAGE_H_
#define _VIPER_CORE_NET_MESSAGE_H_

#include <cstdint>
#include <memory>

namespace viper {
namespace net {

// clang-format off

#define VIPER_NET_MESSAGE_MAGIC                   0xbeeabeaf
#define VIPER_NET_MESSAGE_KEEPALIVE_PING          "KEEPALIVE PING"
#define VIPER_NET_MESSAGE_KEEPALIVE_PONG          "KEEPALIVE PONG"
#define VIPER_NET_MESSAGE_PROTOCOL_KEEPALIVE_PING 0x0000
#define VIPER_NET_MESSAGE_PROTOCOL_KEEPALIVE_PONG 0x0001
#define VIPER_NET_MESSAGE_PROTOCOL_BASE           0x0010

// clang-format on

struct Header
{
    uint32_t _version   = 0;
    uint32_t _msgType   = 0;
    uint32_t _tag       = 0;
    uint32_t _dataSize  = 0;
    uint64_t _magic     = VIPER_NET_MESSAGE_MAGIC;
    uint64_t _sequence  = 0;
    uint64_t _timestamp = 0;
};

class Message final
{
public:
    Message() = default;
    Message(const Header& header, const char* payload, uint32_t payloadSize);
    ~Message();

public:
    enum
    {
        MESSAGE_HEADER_SIZE = sizeof(Header),
        MAX_MESSAGE_SIZE    = 1024 * 1024 * 1024

    };

public:
    void          Reset(const Header& header, const char* payload, uint32_t payloadSize);
    void          ResetHeader(const Header& header);
    void          ResetPayload(const char* data, uint32_t dataSize);
    const Header& GetHeader() const;
    const char*   GetData() const;
    uint32_t      GetDataSize() const;
    const char*   GetPayload() const;
    uint32_t      GetPayloadSize() const;

private:
    Header   _header;
    char*    _data     = nullptr; // The header is already included in the data.
    uint32_t _dataSize = 0;
};

void Hton(Header& header);
void Ntoh(Header& header);

using MessagePtr = std::shared_ptr<Message>;

} // namespace net
} // namespace viper
#endif

