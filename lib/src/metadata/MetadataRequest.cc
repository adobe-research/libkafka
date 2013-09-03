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

#include "MetadataRequest.h"
#include "../ApiConstants.h"

using namespace std;

namespace LibKafka {

MetadataRequest::MetadataRequest(unsigned char *buffer, bool releaseBuffer) : Request(buffer, releaseBuffer)
{
  D(cout.flush() << "--------------MetadataRequest(buffer)\n";)

  // Kafka Protocol: string[] topic_name
  this->topicNameArraySize = this->packet->readInt32();
  this->topicNameArray = new string[this->topicNameArraySize];
  for (int i=0 ; i<this->topicNameArraySize; i++) {
    this->topicNameArray[i] = this->packet->readString();
  }
  this->releaseArrays = true;
}

MetadataRequest::MetadataRequest(int correlationId, string clientId, int topicNameArraySize, string topicNameArray[], bool releaseArrays) : Request(ApiConstants::METADATA_REQUEST_KEY, ApiConstants::API_VERSION, correlationId, clientId)
{
  D(cout.flush() << "--------------MetadataRequest(params)\n";)

  // Kafka Protocol: string[] topicName
  this->topicNameArraySize = topicNameArraySize;
  this->topicNameArray = topicNameArray;
  this->releaseArrays = releaseArrays;
}

MetadataRequest::~MetadataRequest()
{
  if (this->releaseArrays)
  {
    delete[] this->topicNameArray;
  }
}

unsigned char* MetadataRequest::toWireFormat(bool updatePacketSize)
{
  unsigned char* buffer = this->Request::toWireFormat(false);

  D(cout.flush() << "--------------MetadataRequest::toWireFormat()\n";)

  // Kafka Protocol: string[] topicName
  this->packet->writeInt32(this->topicNameArraySize);
  for (int i=0; i<this->topicNameArraySize; i++) {
    this->packet->writeString(this->topicNameArray[i]);
  }

  if (updatePacketSize) this->packet->updatePacketSize();
  return buffer;
}

int MetadataRequest::getWireFormatSize(bool includePacketSize)
{
  D(cout.flush() << "--------------MetadataRequest::getWireFormatSize()\n";)

  // Request.getWireFormatSize
  // string[] topicName

  int size = Request::getWireFormatSize(includePacketSize);
  size += sizeof(int);
  for (int i=0; i<topicNameArraySize; i++) {
    size += sizeof(short int) + topicNameArray[i].length();
  }
  return size;
}

ostream& operator<< (ostream& os, const MetadataRequest& mr)
{
  os << (const Request&)mr;
  os << "MetadataRequest.topicNameArraySize:" << mr.topicNameArraySize << "\n";
  for (int i=0; i<mr.topicNameArraySize; i++) {
    os << "MetadataRequest.topicNameArray[" << i << "]:" << mr.topicNameArray[i] << "\n";
  }
  return os;
}

}; // namespace LibKafka
