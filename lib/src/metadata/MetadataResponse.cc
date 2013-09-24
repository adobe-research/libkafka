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

#include "MetadataResponse.h"
#include "Broker.h"

using namespace std;

namespace LibKafka {

MetadataResponse::MetadataResponse(unsigned char *buffer, bool releaseBuffer) : Response(buffer, releaseBuffer)
{
  D(cout.flush() << "--------------MetadataResponse(buffer)\n";)

  // Kafka Protocol: Broker[] broker
  this->brokerArraySize = this->packet->readInt32();
  this->brokerArray = new Broker*[this->brokerArraySize];
  for (int i=0; i<this->brokerArraySize; i++) {
    this->brokerArray[i] = new Broker(this->packet);
  }

  // Kafka Protocol: TopicMetadata[] topicMetadata
  this->topicMetadataArraySize = this->packet->readInt32();
  this->topicMetadataArray = new TopicMetadata*[this->topicMetadataArraySize];
  for (int i=0; i<this->topicMetadataArraySize; i++) {
    this->topicMetadataArray[i] = new TopicMetadata(this->packet);
  }

  this->releaseArrays = true;
}

MetadataResponse::MetadataResponse(int correlationId, int brokerArraySize, Broker **brokerArray, int topicMetadataArraySize, TopicMetadata **topicMetadataArray, bool releaseArrays) : Response(correlationId)
{
  D(cout.flush() << "--------------MetadataResponse(params)\n";)

  // Kafka Protocol: Broker[] brokers
  this->brokerArraySize = brokerArraySize;
  this->brokerArray = brokerArray;
  this->topicMetadataArraySize = topicMetadataArraySize;
  this->topicMetadataArray = topicMetadataArray;
  this->releaseArrays = releaseArrays;
}

MetadataResponse::~MetadataResponse()
{
  if (releaseArrays) {
    for (int i=0; i<this->brokerArraySize; i++) {
      delete this->brokerArray[i];
    }
    delete[] this->brokerArray;
    for (int i=0; i<this->topicMetadataArraySize; i++) {
      delete this->topicMetadataArray[i];
    }
    delete[] this->topicMetadataArray;
  }
}

unsigned char* MetadataResponse::toWireFormat(bool updatePacketSize)
{
  unsigned char* buffer = this->Response::toWireFormat(false);

  D(cout.flush() << "--------------MetadataResponse::toWireFormat()\n";)

  // Kafka Protocol: Broker[] brokers
  this->packet->writeInt32(this->brokerArraySize);
  for (int i=0; i<this->brokerArraySize; i++) {
    this->brokerArray[i]->packet = this->packet;
    this->brokerArray[i]->toWireFormat(false);
  }
  
  // Kafka Protocol: TopicMetadata[] topicMetadata
  this->packet->writeInt32(this->topicMetadataArraySize);
  for (int i=0; i<this->topicMetadataArraySize; i++) {
    this->topicMetadataArray[i]->packet = this->packet;
    this->topicMetadataArray[i]->toWireFormat(false);
  }

  if (updatePacketSize) this->packet->updatePacketSize();
  return buffer;
}

int MetadataResponse::getWireFormatSize(bool includePacketSize)
{
  D(cout.flush() << "--------------MetadataResponse::getWireFormatSize()\n";)

  // Response.getWireFormatSize
  // brokerArraySize + brokerArraySize*sizeof(Broker)
  // topicMetadataArraySize + topicMetadataArraySize*sizeof(TopicMetadata)

  int size = Response::getWireFormatSize(includePacketSize);
  size += sizeof(int);
  for (int i=0; i<brokerArraySize; i++) {
    size += brokerArray[i]->getWireFormatSize(false);
  }
  size += sizeof(int);
  for (int i=0; i<topicMetadataArraySize; i++) {
    size += topicMetadataArray[i]->getWireFormatSize(false);
  }
  return size;
}

bool MetadataResponse::hasErrorCode()
{
  bool error = false;
  for (int i=0; i<topicMetadataArraySize; i++) {
    error |= topicMetadataArray[i]->hasErrorCode();
  }
  return error;
}

ostream& operator<< (ostream& os, const MetadataResponse& mr)
{
  os << (const Response&)mr;
  os << "MetadataResponse.brokerArraySize:" << mr.brokerArraySize << "\n";
  for (int i=0; i<mr.brokerArraySize; i++) {
    os << "MetadataResponse.brokerArray[" << i << "]:" << *(mr.brokerArray[i]) << "\n";
  }
  os << "MetadataResponse.topicMetadataArraySize:" << mr.topicMetadataArraySize << "\n";
  for (int i=0; i<mr.topicMetadataArraySize; i++) {
    os << "MetadataResponse.topicMetadataArray[" << i << "]:" << *(mr.topicMetadataArray[i]);
  }
  return os;
}

}; // namespace LibKafka
