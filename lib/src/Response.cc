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

#include "Response.h"

using namespace std;

namespace LibKafka {

Response::Response(unsigned char *buffer, bool releaseBuffer) : RequestOrResponse(buffer, releaseBuffer)
{
  D(cout.flush() << "--------------Response(buffer)\n";)
 
  // Kafka Protocol: int correlationId
  this->correlationId = this->packet->readInt32();
}

Response::Response(int correlationId) : RequestOrResponse()
{
  D(cout.flush() << "--------------Response(params)\n";)
 
  // Kafka Protocol: int correlationId
  this->correlationId = correlationId;
}

unsigned char* Response::toWireFormat(bool updatePacketSize)
{
  unsigned char* buffer = this->RequestOrResponse::toWireFormat(false);

  D(cout.flush() << "--------------Response::toWireFormat()\n";)

  // Kafka Protocol: int correlationId
  this->packet->writeInt32(this->correlationId);

  if (updatePacketSize) this->packet->updatePacketSize();
  return buffer;
}

int Response::getWireFormatSize(bool includePacketSize)
{
  D(cout.flush() << "--------------Response::getWireFormatSize()\n";)

  // RequestOrResponse.getWireFormatSize
  // correlationId

  int size = RequestOrResponse::getWireFormatSize(includePacketSize);
  size += sizeof(int);
  return size;
}

ostream& operator<< (ostream& os, const Response& r)
{
  os << (const RequestOrResponse&)r;
  os << "Response.correlationId:" << r.correlationId << "\n";
  return os;
}

}; // namespace LibKafka
