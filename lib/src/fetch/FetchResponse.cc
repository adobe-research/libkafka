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

#include "FetchResponse.h"

using namespace std;

namespace LibKafka {

FetchResponse::FetchResponse(unsigned char *buffer, bool releaseBuffer) : Response(buffer, releaseBuffer)
{
  D(cout.flush() << "--------------FetchResponse(buffer)\n";)

  // Kafka Protocol: TopicNameBlock<FetchResponsePartition>[] fetchResponseTopicArray
  this->fetchResponseTopicArraySize = this->packet->readInt32();
  this->fetchResponseTopicArray = new TopicNameBlock<FetchResponsePartition>*[this->fetchResponseTopicArraySize];
  for (int i=0; i<this->fetchResponseTopicArraySize; i++) {
    this->fetchResponseTopicArray[i] = new TopicNameBlock<FetchResponsePartition>(this->packet);
  }

  this->releaseArrays = true;
}

FetchResponse::FetchResponse(int correlationId, int fetchResponseTopicArraySize, TopicNameBlock<FetchResponsePartition> **fetchResponseTopicArray, bool releaseArrays) : Response(correlationId)
{
  D(cout.flush() << "--------------FetchResponse(params)\n";)

  // Kafka Protocol: TopicNameBlock<FetchResponsePartition>[] fetchResponseTopicArray
  this->fetchResponseTopicArraySize = fetchResponseTopicArraySize;
  this->fetchResponseTopicArray = fetchResponseTopicArray;
  this->releaseArrays = releaseArrays;
}

FetchResponse::~FetchResponse()
{
  if (releaseArrays) {
    for (int i=0; i<this->fetchResponseTopicArraySize; i++) {
      delete this->fetchResponseTopicArray[i];
    }
    delete[] this->fetchResponseTopicArray;
  }
}

unsigned char* FetchResponse::toWireFormat(bool updatePacketSize)
{
  unsigned char* buffer = this->Response::toWireFormat(false);

  D(cout.flush() << "--------------FetchResponse::toWireFormat()\n";)

  // Kafka Protocol: TopicNameBlock<FetchResponsePartition>[] fetchResponseTopicArray
  this->packet->writeInt32(this->fetchResponseTopicArraySize);
  for (int i=0; i<this->fetchResponseTopicArraySize; i++) {
    this->fetchResponseTopicArray[i]->packet = this->packet;
    this->fetchResponseTopicArray[i]->toWireFormat(false);
  }

  if (updatePacketSize) this->packet->updatePacketSize();
  return buffer;
}

int FetchResponse::getWireFormatSize(bool includePacketSize)
{
  D(cout.flush() << "--------------FetchResponse::getWireFormatSize()\n";)

  // Response.getWireFormatSize
  // fetchResponseTopicArraySize + fetchResponseTopicArraySize*sizeof(TopicNameBlock<FetchResponsePartition>)

  int size = Response::getWireFormatSize(includePacketSize);
  size += sizeof(int);
  for (int i=0; i<fetchResponseTopicArraySize; i++) {
    size += fetchResponseTopicArray[i]->getWireFormatSize(false);
  }
  return size;
}

bool FetchResponse::hasErrorCode()
{
  bool error = false;
  for (int i=0; i<fetchResponseTopicArraySize; i++) {
    error |= fetchResponseTopicArray[i]->hasErrorCode();
  }
  return error;
}

ostream& operator<< (ostream& os, const FetchResponse& pr)
{
  os << (const Response&)pr;
  os << "FetchResponse.fetchResponseTopicArraySize:" << pr.fetchResponseTopicArraySize << "\n";
  for (int i=0; i<pr.fetchResponseTopicArraySize; i++) {
    os << "FetchResponse.fetchResponseTopicArray[" << i << "]:" << *(pr.fetchResponseTopicArray[i]) << "\n";
  }
  return os;
}

}; // namespace LibKafka
