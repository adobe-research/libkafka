///////////////////////////////////////////////////////////////////////////
//
// libkafka - C/C++ client for Apache Kafka v0.8+
//
// David Tompkins -- 8/8/2013
// tompkins@adobe_dot_com
//
///////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2013 Adobe Systems Incorporated. All rights reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
///////////////////////////////////////////////////////////////////////////

#ifndef MESSAGE_H
#define MESSAGE_H

#include <string>

#include "Debug.h"
#include "Packet.h"
#include "WireFormatter.h"
#include "PacketWriter.h"

namespace LibKafka {

class Message: public WireFormatter, public PacketWriter
{
  public:

    const static unsigned char COMPRESSION_MASK = 0x7;

    int crc;
    unsigned char magicByte;
    unsigned char attributes;
    int keyLength;
    unsigned char* key;
    int valueLength;
    unsigned char* value;
    int compressedValueLength;

    int64_t offset;

    Message(Packet *packet, int64_t offset = -1);
    Message(int crc, unsigned char magicByte, unsigned char attributes, int keyLength, unsigned char* key, int valueLength, unsigned char* value, int64_t offset = -1, bool releaseArrays = false);
    ~Message();

    unsigned char* toWireFormat(bool updatePacketSize = true);
    int getWireFormatSize(bool includePacketSize = false);

    void setCompression(int codec);
    bool hasCompression();

  private:

    bool releaseArrays;
};

std::ostream& operator<< (std::ostream& os, const Message& t);
inline bool operator==(const Message& lhs, const Message& rhs) { return ((lhs.crc==rhs.crc)&&(lhs.magicByte==rhs.magicByte)&&(lhs.attributes==rhs.attributes)&&(lhs.keyLength==rhs.keyLength)&&(lhs.valueLength==rhs.valueLength)); }
inline bool operator!=(const Message& lhs, const Message& rhs) { return !operator==(lhs,rhs); }

}; // namespace LibKafka

#endif /* MESSAGE_H */
