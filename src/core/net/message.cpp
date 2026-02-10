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

#include "core/net/message.h"

#include <cstdint>
#include <cstring>
#include <new>

#include <arpa/inet.h>
#include <endian.h>
#include <memory.h>
#include <stdlib.h>

namespace viper {
namespace net {

Message::Message(const Header& header, const char* payload, uint32_t payloadSize)
{
    Reset(header, payload, payloadSize);
}

Message::~Message()
{
    if (_data)
    {
        free(_data);
        _data = nullptr;
    }

    _dataSize = 0;
}

void Message::Reset(const Header& header, const char* payload, uint32_t payloadSize)
{
    ResetHeader(header);
    ResetPayload(payload, payloadSize);
}

void Message::ResetHeader(const Header& header)
{
    memcpy(&_header, &header, MESSAGE_HEADER_SIZE);
}

void Message::ResetPayload(const char* payload, uint32_t payloadSize)
{
    if (_data)
    {
        free(_data);
        _data = nullptr;
    }

    _dataSize = MESSAGE_HEADER_SIZE + payloadSize;
    _data     = (char*)malloc(sizeof(char) * _dataSize);
    if (!_data)
    {
        throw std::bad_alloc();
    }

    memcpy(_data, &_header, MESSAGE_HEADER_SIZE);
    memcpy(_data + MESSAGE_HEADER_SIZE, payload, payloadSize);
}

const Header& Message::GetHeader() const
{
    return _header;
}

const char* Message::GetData() const
{
    return _data;
}

uint32_t Message::GetDataSize() const
{
    return _dataSize;
}

const char* Message::GetPayload() const
{
    return _data + MESSAGE_HEADER_SIZE;
}

uint32_t Message::GetPayloadSize() const
{
    return _dataSize - MESSAGE_HEADER_SIZE;
}

void Hton(Header& header)
{
    header._version  = htonl(header._version);
    header._msgType  = htonl(header._msgType);
    header._tag      = htonl(header._tag);
    header._dataSize = htonl(header._dataSize);

    // Net Byte Order is Big-Endian

    if (__BYTE_ORDER == __LITTLE_ENDIAN)
    {
        header._magic     = htobe64(header._magic);
        header._sequence  = htobe64(header._sequence);
        header._timestamp = htobe64(header._timestamp);
    }
}

void Ntoh(Header& header)
{
    header._version  = ntohl(header._version);
    header._msgType  = ntohl(header._msgType);
    header._tag      = ntohl(header._tag);
    header._dataSize = ntohl(header._dataSize);

    // Net Byte Order is Big-Endian

    if (__BYTE_ORDER == __LITTLE_ENDIAN)
    {
        header._magic     = be64toh(header._magic);
        header._sequence  = be64toh(header._sequence);
        header._timestamp = be64toh(header._timestamp);
    }
}

} // namespace net
} // namespace viper

