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

#include "OffsetPartition.h"
#include "../ApiConstants.h"
#include "../Util.h"

using namespace std;

namespace LibKafka {

  OffsetPartition::OffsetPartition(Packet *packet) : WireFormatter(), PacketWriter(packet)
  {
    D(cout.flush() << "--------------OffsetPartition(buffer)\n";)

    // Kafka Protocol: int partition
    this->partition = this->packet->readInt32();

    // Kafka Protocol: int64_t time
    this->time = this->packet->readInt64();
    
    // Kafka Protocol: int maxNumberOfOffsets
    this->maxNumberOfOffsets = this->packet->readInt32();
  }

  OffsetPartition::OffsetPartition(int partition, int64_t time, int maxNumberOfOffsets) : WireFormatter(), PacketWriter()
  {
    D(cout.flush() << "--------------OffsetPartition(params)\n";)

    this->partition = partition;
    this->time = time;
    this->maxNumberOfOffsets = maxNumberOfOffsets;
  }

  OffsetPartition::~OffsetPartition()
  {
  }

  unsigned char* OffsetPartition::toWireFormat(bool updatePacketSize)
  {
    D(cout.flush() << "--------------OffsetPartition::toWireFormat()\n";)

    // Kafka Protocol: int partition
    this->packet->writeInt32(this->partition);

    // Kafka Protocol: int64_t time
    this->packet->writeInt64(this->time);

    // Kafka Protocol: int maxNumberOfOffsets
    this->packet->writeInt32(this->maxNumberOfOffsets);

    if (updatePacketSize) this->packet->updatePacketSize();
    return this->packet->getBuffer();
  }

  int OffsetPartition::getWireFormatSize(bool includePacketSize)
  {
    D(cout.flush() << "--------------OffsetPartition::getWireFormatSize()\n";)

    // Packet.size
    // partition + time + maxNumberOfOffsets

    int size = 0;
    if (includePacketSize) size += sizeof(int);
    size += sizeof(int) + sizeof(int64_t) + sizeof(int);
    return size;
  }

  ostream& operator<< (ostream& os, const OffsetPartition& pm)
  {
    os << "OffsetPartition.partition:" << pm.partition << "\n";
    os << "OffsetPartition.time:" << pm.time << "\n";
    os << "OffsetPartition.maxNumberOfOffsets:" << pm.maxNumberOfOffsets << "\n";
    return os;
  }

}; // namespace LibKafka
