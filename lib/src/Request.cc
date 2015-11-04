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

#include "Request.h"

using namespace std;

namespace LibKafka {

Request::Request(unsigned char *buffer, bool releaseBuffer) : RequestOrResponse(buffer, releaseBuffer)
{
  D(cout.flush() << "--------------Request(buffer)\n";)

  // Kafka Protocol: short int apiKey
  this->apiKey = this->packet->readInt16();

  // Kafka Protocol: short int apiVersion
  this->apiVersion = this->packet->readInt16();

  // Kafka Protocol: int correlationId
  this->correlationId = this->packet->readInt32();

  // Kafka Protocol: kafka string clientId
  this->clientId = this->packet->readString();
}

Request::Request(short int apiKey, short int apiVersion, int correlationId, string clientId) : RequestOrResponse()
{
  D(cout.flush() << "--------------Request(params)\n";)

  this->apiKey = apiKey;
  this->apiVersion = apiVersion;
  this->correlationId = correlationId;
  this->clientId = clientId;
}

Request::Request(short int apiKey, short int apiVersion, int correlationId, string clientId, long bufferSize) : RequestOrResponse(bufferSize)
{
  D(cout.flush() << "--------------Request(params)\n";)

  this->apiKey = apiKey;
  this->apiVersion = apiVersion;
  this->correlationId = correlationId;
  this->clientId = clientId;
}


unsigned char* Request::toWireFormat(bool updatePacketSize)
{
  unsigned char* buffer = this->RequestOrResponse::toWireFormat(false);

  D(cout.flush() << "--------------Request::toWireFormat()\n";)

  // Kafka Protocol: short int apiKey
  this->packet->writeInt16(this->apiKey);

  // Kafka Protocol: short int apiVersion
  this->packet->writeInt16(this->apiVersion);

  // Kafka Protocol: int correlationId
  this->packet->writeInt32(this->correlationId);

  // Kafka Protocol: kafka string clientId
  this->packet->writeString(this->clientId);

  if (updatePacketSize) this->packet->updatePacketSize();
  return buffer;
}

int Request::getWireFormatSize(bool includePacketSize)
{
  D(cout.flush() << "--------------Request::getWireFormatSize()\n";)

  // RequestOrResponse.getWireFormatSize
  // apiKey + apiVersion + correlationId + clientId

  int size = RequestOrResponse::getWireFormatSize(includePacketSize);
  size += sizeof(short int) + sizeof(short int) + sizeof(int) + sizeof(short int) + clientId.length();
  return size;
}

ostream& operator<< (ostream& os, const Request& r)
{
  os << (const RequestOrResponse&)r;
  os << "Request.apiKey:" << r.apiKey << "\n";
  os << "Request.apiVersion:" << r.apiVersion << "\n";
  os << "Request.correlationId:" << r.correlationId << "\n";
  os << "Request.clientId:" << r.clientId << "\n";
  return os;
}

}; // namespace LibKafka
