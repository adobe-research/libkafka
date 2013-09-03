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

#include "FetchRequest.h"
#include "../ApiConstants.h"

using namespace std;

namespace LibKafka {

  FetchRequest::FetchRequest(unsigned char *buffer, bool releaseBuffer) : Request(buffer, releaseBuffer)
  {
    D(cout.flush() << "--------------FetchRequest(buffer)\n";)

    // Kafka Protocol: int replicaId
    this->replicaId = this->packet->readInt32();

    // Kafka Protocol: int maxWaitTime
    this->maxWaitTime = this->packet->readInt32();

    // Kafka Protocol: int minBytes
    this->minBytes = this->packet->readInt32();

    // Kafka Protocol: TopicNameBlock[] (note TopicNameBlock is not a protocol-defined structure)
    this->fetchTopicArraySize = this->packet->readInt32();
    this->fetchTopicArray = new TopicNameBlock<FetchPartition>*[this->fetchTopicArraySize];
    for (int i=0; i<this->fetchTopicArraySize; i++) {
      this->fetchTopicArray[i] = new TopicNameBlock<FetchPartition>(this->packet);
    }
    this->releaseArrays = true;
  }

  FetchRequest::FetchRequest(int correlationId, std::string clientId, int replicaId, int maxWaitTime, int minBytes, int fetchTopicArraySize, TopicNameBlock<FetchPartition>** fetchTopicArray, bool releaseArrays) : Request(ApiConstants::FETCH_REQUEST_KEY, ApiConstants::API_VERSION, correlationId, clientId)
  {
    D(cout.flush() << "--------------FetchRequest(params)\n";)

    this->replicaId = replicaId;
    this->maxWaitTime = maxWaitTime;
    this->minBytes = minBytes;
    this->fetchTopicArraySize = fetchTopicArraySize;
    this->fetchTopicArray = fetchTopicArray;
    this->releaseArrays = releaseArrays;
  }

  FetchRequest::~FetchRequest()
  {
    if (this->releaseArrays)
    {
      for (int i=0; i<this->fetchTopicArraySize; i++) {
	delete this->fetchTopicArray[i];
      }
      delete[] this->fetchTopicArray;
    }
  }

  unsigned char* FetchRequest::toWireFormat(bool updatePacketSize)
  {
    unsigned char* buffer = this->Request::toWireFormat(false);

    D(cout.flush() << "--------------FetchRequest::toWireFormat()\n";)
    
    // Kafka Protocol: short int replicaId
    this->packet->writeInt32(this->replicaId);

    // Kafka Protocol: int maxWaitTime
    this->packet->writeInt32(this->maxWaitTime);

    // Kafka Protocol: int minBytes
    this->packet->writeInt32(this->minBytes);

    // Kafka Protocol: TopicNameBlock[] (note TopicNameBlock is not a protocol-defined structure)
    this->packet->writeInt32(this->fetchTopicArraySize);
    for (int i=0; i<this->fetchTopicArraySize; i++) {
      this->fetchTopicArray[i]->packet = this->packet;
      this->fetchTopicArray[i]->toWireFormat(false);
    }

    if (updatePacketSize) this->packet->updatePacketSize();
    return buffer;
  }

  int FetchRequest::getWireFormatSize(bool includePacketSize)
  {
    D(cout.flush() << "--------------FetchRequest::getWireFormatSize()\n";)

    // Request.getWireFormatSize
    // replicaId + maxWaitTime + minBytes + produceTopicArraySize*sizeof(TopicNameBlock<FetchPartition>)

    int size = Request::getWireFormatSize(includePacketSize);
    size += sizeof(int) + sizeof(int) + sizeof(int);
    size += sizeof(int);
    for (int i=0; i<fetchTopicArraySize; i++) {
      size += fetchTopicArray[i]->getWireFormatSize(false);
    }
    return size;
  }

  ostream& operator<< (ostream& os, const FetchRequest& fr)
  {
    os << (const Request&)fr;
    os << "FetchRequest.replicaId:" << fr.replicaId << "\n";
    os << "FetchRequest.maxWaitTime:" << fr.maxWaitTime << "\n";
    os << "FetchRequest.minBytes:" << fr.minBytes << "\n";
    os << "FetchRequest.fetchTopicArraySize:" << fr.fetchTopicArraySize << "\n";
    for (int i=0; i<fr.fetchTopicArraySize; i++) {
      os << "FetchRequest.fetchTopicArray[" << i << "]:" << *(fr.fetchTopicArray[i]) << "\n";
    }
    return os;
  }

}; // namespace LibKafka
