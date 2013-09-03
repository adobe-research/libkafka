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

#include <iostream>

#include "OffsetRequest.h"
#include "../ApiConstants.h"

using namespace std;

namespace LibKafka {

  OffsetRequest::OffsetRequest(unsigned char *buffer, bool releaseBuffer) : Request(buffer, releaseBuffer)
  {
    D(cout.flush() << "--------------OffsetRequest(buffer)\n";)

    // Kafka Protocol: int replicaId
    this->replicaId = this->packet->readInt32();

    // Kafka Protocol: TopicNameBlock[] (note TopicNameBlock is not a protocol-defined structure)
    this->offsetTopicArraySize = this->packet->readInt32();
    this->offsetTopicArray = new TopicNameBlock<OffsetPartition>*[this->offsetTopicArraySize];
    for (int i=0; i<this->offsetTopicArraySize; i++) {
      this->offsetTopicArray[i] = new TopicNameBlock<OffsetPartition>(this->packet);
    }
    this->releaseArrays = true;
  }

  OffsetRequest::OffsetRequest(int correlationId, std::string clientId, int replicaId, int offsetTopicArraySize, TopicNameBlock<OffsetPartition>** offsetTopicArray, bool releaseArrays) : Request(ApiConstants::OFFSET_REQUEST_KEY, ApiConstants::API_VERSION, correlationId, clientId)
  {
    D(cout.flush() << "--------------OffsetRequest(params)\n";)

    this->replicaId = replicaId;
    this->offsetTopicArraySize = offsetTopicArraySize;
    this->offsetTopicArray = offsetTopicArray;
    this->releaseArrays = releaseArrays;
  }

  OffsetRequest::~OffsetRequest()
  {
    if (this->releaseArrays)
    {
      for (int i=0; i<this->offsetTopicArraySize; i++) {
	delete this->offsetTopicArray[i];
      }
      delete[] this->offsetTopicArray;
    }
  }

  unsigned char* OffsetRequest::toWireFormat(bool updatePacketSize)
  {
    unsigned char* buffer = this->Request::toWireFormat(false);

    D(cout.flush() << "--------------OffsetRequest::toWireFormat()\n";)
    
    // Kafka Protocol: int replicaId
    this->packet->writeInt32(this->replicaId);

    // Kafka Protocol: TopicNameBlock[] (note TopicNameBlock is not a protocol-defined structure)
    this->packet->writeInt32(this->offsetTopicArraySize);
    for (int i=0; i<this->offsetTopicArraySize; i++) {
      this->offsetTopicArray[i]->packet = this->packet;
      this->offsetTopicArray[i]->toWireFormat(false);
    }

    if (updatePacketSize) this->packet->updatePacketSize();
    return buffer;
  }

  int OffsetRequest::getWireFormatSize(bool includePacketSize)
  {
    D(cout.flush() << "--------------OffsetRequest::getWireFormatSize()\n";)

    // Request.getWireFormatSize
    // replicaId + produceTopicArraySize*sizeof(TopicNameBlock<OffsetPartition>)

    int size = Request::getWireFormatSize(includePacketSize);
    size += sizeof(int);
    size += sizeof(int);
    for (int i=0; i<offsetTopicArraySize; i++) {
      size += offsetTopicArray[i]->getWireFormatSize(false);
    }
    return size;
  }

  ostream& operator<< (ostream& os, const OffsetRequest& fr)
  {
    os << (const Request&)fr;
    os << "OffsetRequest.replicaId:" << fr.replicaId << "\n";
    os << "OffsetRequest.offsetTopicArraySize:" << fr.offsetTopicArraySize << "\n";
    for (int i=0; i<fr.offsetTopicArraySize; i++) {
      os << "OffsetRequest.offsetTopicArray[" << i << "]:" << *(fr.offsetTopicArray[i]) << "\n";
    }
    return os;
  }

}; // namespace LibKafka
