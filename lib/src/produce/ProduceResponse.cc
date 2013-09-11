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
#include <vector>

#include "ProduceResponse.h"

using namespace std;

namespace LibKafka {

ProduceResponse::ProduceResponse(unsigned char *buffer, bool releaseBuffer) : Response(buffer, releaseBuffer)
{
  D(cout.flush() << "--------------ProduceResponse(buffer)\n";)

  // Kafka Protocol: TopicNameBlock<ProduceResponsePartition>[] produceResponseTopicArray
  this->produceResponseTopicArraySize = this->packet->readInt32();
  this->produceResponseTopicArray = new TopicNameBlock<ProduceResponsePartition>*[this->produceResponseTopicArraySize];
  for (int i=0; i<this->produceResponseTopicArraySize; i++) {
    this->produceResponseTopicArray[i] = new TopicNameBlock<ProduceResponsePartition>(this->packet);
  }

  this->releaseArrays = true;
}

ProduceResponse::ProduceResponse(int correlationId, int produceResponseTopicArraySize, TopicNameBlock<ProduceResponsePartition> **produceResponseTopicArray, bool releaseArrays) : Response(correlationId)
{
  D(cout.flush() << "--------------ProduceResponse(params)\n";)

  // Kafka Protocol: TopicNameBlock<ProduceResponsePartition>[] produceResponseTopicArray
  this->produceResponseTopicArraySize = produceResponseTopicArraySize;
  this->produceResponseTopicArray = produceResponseTopicArray;
  this->releaseArrays = releaseArrays;
}

ProduceResponse::~ProduceResponse()
{
  if (releaseArrays) {
    for (int i=0; i<this->produceResponseTopicArraySize; i++) {
      delete this->produceResponseTopicArray[i];
    }
    delete[] this->produceResponseTopicArray;
  }
}

unsigned char* ProduceResponse::toWireFormat(bool updatePacketSize)
{
  unsigned char* buffer = this->Response::toWireFormat(false);

  D(cout.flush() << "--------------ProduceResponse::toWireFormat()\n";)

  // Kafka Protocol: TopicNameBlock<ProduceResponsePartition>[] produceResponseTopicArray
  this->packet->writeInt32(this->produceResponseTopicArraySize);
  for (int i=0; i<this->produceResponseTopicArraySize; i++) {
    this->produceResponseTopicArray[i]->packet = this->packet;
    this->produceResponseTopicArray[i]->toWireFormat(false);
  }

  if (updatePacketSize) this->packet->updatePacketSize();
  return buffer;
}

int ProduceResponse::getWireFormatSize(bool includePacketSize)
{
  D(cout.flush() << "--------------ProduceResponse::getWireFormatSize()\n";)

  // Response.getWireFormatSize
  // produceResponseTopicArraySize + produceResponseTopicArraySize*sizeof(TopicNameBlock<ProduceResponsePartition>)

  int size = Response::getWireFormatSize(includePacketSize);
  size += sizeof(int);
  for (int i=0; i<produceResponseTopicArraySize; i++) {
    size += produceResponseTopicArray[i]->getWireFormatSize(false);
  }
  return size;
}

ostream& operator<< (ostream& os, const ProduceResponse& pr)
{
  os << (const Response&)pr;
  os << "ProduceResponse.produceResponseTopicArraySize:" << pr.produceResponseTopicArraySize << "\n";
  for (int i=0; i<pr.produceResponseTopicArraySize; i++) {
    os << "ProduceResponse.produceResponseTopicArray[" << i << "]:" << *(pr.produceResponseTopicArray[i]) << "\n";
  }
  return os;
}

}; // namespace LibKafka
