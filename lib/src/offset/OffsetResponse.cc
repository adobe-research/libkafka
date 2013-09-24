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

#include "OffsetResponse.h"

using namespace std;

namespace LibKafka {

OffsetResponse::OffsetResponse(unsigned char *buffer, bool releaseBuffer) : Response(buffer, releaseBuffer)
{
  D(cout.flush() << "--------------OffsetResponse(buffer)\n";)

  // Kafka Protocol: TopicNameBlock<OffsetResponsePartition>[] offsetResponseTopicArray
  this->offsetResponseTopicArraySize = this->packet->readInt32();
  this->offsetResponseTopicArray = new TopicNameBlock<OffsetResponsePartition>*[this->offsetResponseTopicArraySize];
  for (int i=0; i<this->offsetResponseTopicArraySize; i++) {
    this->offsetResponseTopicArray[i] = new TopicNameBlock<OffsetResponsePartition>(this->packet);
  }

  this->releaseArrays = true;
}

OffsetResponse::OffsetResponse(int correlationId, int offsetResponseTopicArraySize, TopicNameBlock<OffsetResponsePartition> **offsetResponseTopicArray, bool releaseArrays) : Response(correlationId)
{
  D(cout.flush() << "--------------OffsetResponse(params)\n";)

  // Kafka Protocol: TopicNameBlock<OffsetResponsePartition>[] offsetResponseTopicArray
  this->offsetResponseTopicArraySize = offsetResponseTopicArraySize;
  this->offsetResponseTopicArray = offsetResponseTopicArray;
  this->releaseArrays = releaseArrays;
}

OffsetResponse::~OffsetResponse()
{
  if (releaseArrays) {
    for (int i=0; i<this->offsetResponseTopicArraySize; i++) {
      delete this->offsetResponseTopicArray[i];
    }
    delete[] this->offsetResponseTopicArray;
  }
}

unsigned char* OffsetResponse::toWireFormat(bool updatePacketSize)
{
  unsigned char* buffer = this->Response::toWireFormat(false);

  D(cout.flush() << "--------------OffsetResponse::toWireFormat()\n";)

  // Kafka Protocol: TopicNameBlock<OffsetResponsePartition>[] offsetResponseTopicArray
  this->packet->writeInt32(this->offsetResponseTopicArraySize);
  for (int i=0; i<this->offsetResponseTopicArraySize; i++) {
    this->offsetResponseTopicArray[i]->packet = this->packet;
    this->offsetResponseTopicArray[i]->toWireFormat(false);
  }

  if (updatePacketSize) this->packet->updatePacketSize();
  return buffer;
}

int OffsetResponse::getWireFormatSize(bool includePacketSize)
{
  D(cout.flush() << "--------------OffsetResponse::getWireFormatSize()\n";)

  // Response.getWireFormatSize
  // offsetResponseTopicArraySize + offsetResponseTopicArraySize*sizeof(TopicNameBlock<OffsetResponsePartition>)

  int size = Response::getWireFormatSize(includePacketSize);
  size += sizeof(int);
  for (int i=0; i<offsetResponseTopicArraySize; i++) {
    size += offsetResponseTopicArray[i]->getWireFormatSize(false);
  }
  return size;
}

bool OffsetResponse::hasErrorCode()
{
  bool error = false;
  for (int i=0; i<offsetResponseTopicArraySize; i++) {
    error |= offsetResponseTopicArray[i]->hasErrorCode();
  }
  return error;
}

ostream& operator<< (ostream& os, const OffsetResponse& pr)
{
  os << (const Response&)pr;
  os << "OffsetResponse.offsetResponseTopicArraySize:" << pr.offsetResponseTopicArraySize << "\n";
  for (int i=0; i<pr.offsetResponseTopicArraySize; i++) {
    os << "OffsetResponse.offsetResponseTopicArray[" << i << "]:" << *(pr.offsetResponseTopicArray[i]) << "\n";
  }
  return os;
}

}; // namespace LibKafka
