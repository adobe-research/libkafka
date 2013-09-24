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

#include <string>
#include <iostream>

#include "PartitionMetadata.h"
#include "../ApiConstants.h"
#include "../Util.h"

using namespace std;

namespace LibKafka {

  PartitionMetadata::PartitionMetadata(Packet *packet) : WireFormatter(), PacketWriter(packet), ErrorHandler()
  {
    D(cout.flush() << "--------------PartitionMetadata(buffer)\n";)

      // Kafka Protocol: short int partitionErrorCode
      this->partitionErrorCode = this->packet->readInt16();

    // Kafka Protocol: int partitionId
    this->partitionId = this->packet->readInt32();

    // Kafka Protocol: int leader
    this->leader = this->packet->readInt32();

    // Kafka Protocol: int[] replica
    this->replicaArraySize = this->packet->readInt32();
    this->replicaArray = new int[this->replicaArraySize];
    for (int i=0; i<this->replicaArraySize; i++) {
      this->replicaArray[i] = this->packet->readInt32();
    }

    // Kafka Protocol: int[] isr
    this->isrArraySize = this->packet->readInt32();
    this->isrArray = new int[this->isrArraySize];
    for (int i=0; i<this->isrArraySize; i++) {
      this->isrArray[i] = this->packet->readInt32();
    }

    this->releaseArrays = true;
  }

  PartitionMetadata::PartitionMetadata(short int partitionErrorCode, int partitionId, int leader, int replicaArraySize, int *replicaArray, int isrArraySize, int *isrArray, bool releaseArrays) : WireFormatter(), PacketWriter(), ErrorHandler()
  {
    D(cout.flush() << "--------------PartitionMetadata(params)\n";)

      this->partitionErrorCode = partitionErrorCode;
    this->partitionId = partitionId;
    this->leader = leader;
    this->replicaArraySize = replicaArraySize;
    this->replicaArray = replicaArray;
    this->isrArraySize = isrArraySize;
    this->isrArray = isrArray;
    this->releaseArrays = releaseArrays;
  }

  PartitionMetadata::~PartitionMetadata()
  {
    if (this->releaseArrays)
    {
      delete[] this->replicaArray;
      delete[] this->isrArray;
    }
  }

  unsigned char* PartitionMetadata::toWireFormat(bool updatePacketSize)
  {
    D(cout.flush() << "--------------PartitionMetadata::toWireFormat()\n";)

      // Kafka Protocol: short int partitionErrorCode
      this->packet->writeInt16(this->partitionErrorCode);

    // Kafka Protocol: int partitionId
    this->packet->writeInt32(this->partitionId);

    // Kafka Protocol: int leader
    this->packet->writeInt32(this->leader);

    // Kafka Protocol: int[] replica
    this->packet->writeInt32(this->replicaArraySize);
    for (int i=0; i<this->replicaArraySize; i++) {
      this->packet->writeInt32(this->replicaArray[i]);
    }

    // Kafka Protocol: int[] isr
    this->packet->writeInt32(this->isrArraySize);
    for (int i=0; i<this->isrArraySize; i++) {
      this->packet->writeInt32(this->isrArray[i]);
    }

    if (updatePacketSize) this->packet->updatePacketSize();
    return this->packet->getBuffer();
  }

  int PartitionMetadata::getWireFormatSize(bool includePacketSize)
  {
    D(cout.flush() << "--------------PartitionMetadata::getWireFormatSize()\n";)

      // Packet.size
      // partitionErrorCode + partitionId + leader
      // replicaArraySize + replicaArraySize*int32
      // isrArraySize + isrArraySize*int32

      int size = 0;
    if (includePacketSize) size += sizeof(int);
    size += sizeof(short int) + sizeof(int) + sizeof(int);
    size += sizeof(int) + (replicaArraySize * sizeof(int));
    size += sizeof(int) + (isrArraySize * sizeof(int));
    return size;
  }

  bool PartitionMetadata::hasErrorCode()
  {
    return (this->partitionErrorCode != ApiConstants::ERRORCODE_NO_ERROR);
  }

  ostream& operator<< (ostream& os, const PartitionMetadata& pm)
  {
    os << "PartitionMetadata.partitionErrorCode:" << pm.partitionErrorCode << ":" << ApiConstants::getErrorString(pm.partitionErrorCode) << "\n";
    os << "PartitionMetadata.partitionId:" << pm.partitionId << "\n";
    os << "PartitionMetadata.leader:" << pm.leader << "\n";
    os << "PartitionMetadata.replicaArray:" << intArrayToString(pm.replicaArray, pm.replicaArraySize) << "\n";
    os << "PartitionMetadata.isrArray:" << intArrayToString(pm.isrArray, pm.isrArraySize) << "\n";
    return os;
  }

}; // namespace LibKafka
