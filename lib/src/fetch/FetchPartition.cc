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

#include <cstdint>
#include <string>
#include <iostream>

#include "FetchPartition.h"
#include "../ApiConstants.h"
#include "../Util.h"

using namespace std;

namespace LibKafka {

  FetchPartition::FetchPartition(Packet *packet) : WireFormatter(), PacketWriter(packet)
  {
    D(cout.flush() << "--------------FetchPartition(buffer)\n";)

    // Kafka Protocol: int partition
    this->partition = this->packet->readInt32();

    // Kafka Protocol: int fetchOffset
    this->fetchOffset = this->packet->readInt64();
    
    // Kafka Protocol: int maxBytes
    this->maxBytes = this->packet->readInt32();
  }

  FetchPartition::FetchPartition(int partition, int64_t fetchOffset, int maxBytes) : WireFormatter(), PacketWriter()
  {
    D(cout.flush() << "--------------FetchPartition(params)\n";)

    this->partition = partition;
    this->fetchOffset = fetchOffset;
    this->maxBytes = maxBytes;
  }

  FetchPartition::~FetchPartition()
  {
  }

  unsigned char* FetchPartition::toWireFormat(bool updatePacketSize)
  {
    D(cout.flush() << "--------------FetchPartition::toWireFormat()\n";)

    // Kafka Protocol: int partition
    this->packet->writeInt32(this->partition);

    // Kafka Protocol: int fetchOffset
    this->packet->writeInt64(this->fetchOffset);

    // Kafka Protocol: int maxBytes
    this->packet->writeInt32(this->maxBytes);

    if (updatePacketSize) this->packet->updatePacketSize();
    return this->packet->getBuffer();
  }

  int FetchPartition::getWireFormatSize(bool includePacketSize)
  {
    D(cout.flush() << "--------------FetchPartition::getWireFormatSize()\n";)

    // Packet.size
    // partition + fetchOffset + maxBytes

    int size = 0;
    if (includePacketSize) size += sizeof(int);
    size += sizeof(int) + sizeof(int64_t) + sizeof(int);
    return size;
  }

  ostream& operator<< (ostream& os, const FetchPartition& pm)
  {
    os << "FetchPartition.partition:" << pm.partition << "\n";
    os << "FetchPartition.fetchOffset:" << pm.fetchOffset << "\n";
    os << "FetchPartition.maxBytes:" << pm.maxBytes << "\n";
    return os;
  }

}; // namespace LibKafka
