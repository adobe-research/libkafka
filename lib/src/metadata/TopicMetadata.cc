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

#include "TopicMetadata.h"
#include "PartitionMetadata.h"
#include "../ApiConstants.h"

using namespace std;

namespace LibKafka {

  TopicMetadata::TopicMetadata(Packet *packet) : WireFormatter(), PacketWriter(packet)
  {
    D(cout.flush() << "--------------TopicMetadata(buffer)\n";)

      // Kafka Protocol: short int topicErrorCode
      this->topicErrorCode = this->packet->readInt16();

    // Kafka Protocol: kafka string topicName
    this->topicName = this->packet->readString();

    // Kafka Protocol: PartitionMetadata[]
    this->partitionMetadataArraySize = this->packet->readInt32();
    this->partitionMetadataArray = new PartitionMetadata*[this->partitionMetadataArraySize];
    for (int i=0; i<this->partitionMetadataArraySize; i++) {
      this->partitionMetadataArray[i] = new PartitionMetadata(this->packet);
    }

    this->releaseArrays = true;
  }

  TopicMetadata::TopicMetadata(short int topicErrorCode, string topicName, int partitionMetadataArraySize, PartitionMetadata **partitionMetadataArray, bool releaseArrays) : WireFormatter(), PacketWriter()
  {
    D(cout.flush() << "--------------TopicMetadata(params)\n";)

      this->topicErrorCode = topicErrorCode;
    this->topicName = topicName;
    this->partitionMetadataArraySize = partitionMetadataArraySize;
    this->partitionMetadataArray = partitionMetadataArray;
    this->releaseArrays = releaseArrays;
  }

  TopicMetadata::~TopicMetadata()
  {
    if (this->releaseArrays)
    {
      for (int i=0; i<this->partitionMetadataArraySize; i++) {
	delete this->partitionMetadataArray[i];
      }
      delete[] this->partitionMetadataArray;
    }
  }

  unsigned char* TopicMetadata::toWireFormat(bool updatePacketSize)
  {
    D(cout.flush() << "--------------TopicMetadata::toWireFormat()\n";)

      // Kafka Protocol: short int topicErrorCode
      this->packet->writeInt16(this->topicErrorCode);

    // Kafka Protocol: kafka string topicName
    this->packet->writeString(this->topicName);

    // Kafka Protocol: PartitionMetadata[]
    this->packet->writeInt32(this->partitionMetadataArraySize);
    for (int i=0; i<this->partitionMetadataArraySize; i++) {
      this->partitionMetadataArray[i]->packet = this->packet;
      this->partitionMetadataArray[i]->toWireFormat(false);
    }

    if (updatePacketSize) this->packet->updatePacketSize();
    return this->packet->getBuffer();
  }

  int TopicMetadata::getWireFormatSize(bool includePacketSize)
  {
    D(cout.flush() << "--------------TopicMetadata::getWireFormatSize()\n";)

      // Packet.size
      // topicErrorCode + topicName + partitionMetadataArraySize
      // partitionMetadataArraySize*PartitionMetadata

      int size = 0;
    if (includePacketSize) size += sizeof(int);
    size += sizeof(short int) + sizeof(short int) + this->topicName.length() + sizeof(int);
    for (int i=0; i<partitionMetadataArraySize; i++) {
      size += partitionMetadataArray[i]->getWireFormatSize(false);
    }
    return size;
  }

  ostream& operator<< (ostream& os, const TopicMetadata& tm)
  {
    os << "TopicMetadata.topicName:" << tm.topicName << "\n";
    os << "TopicMetadata.topicErrorCode:" << tm.topicErrorCode << ":" << ApiConstants::getErrorString(tm.topicErrorCode) << "\n";
    os << "TopicMetadata.partitionMetadataArraySize:" << tm.partitionMetadataArraySize << "\n";
    for (int i=0; i<tm.partitionMetadataArraySize; i++) {
      os << "TopicMetadata.partitionMetadataArray[" << i << "]:" << *(tm.partitionMetadataArray[i]);
    }
    return os;
  }

}; // namespace LibKafka
