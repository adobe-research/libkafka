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

#include "RequestOrResponse.h"

using namespace std;

namespace LibKafka {

RequestOrResponse::RequestOrResponse(unsigned char *buffer, bool releaseBuffer) : WireFormatter()
{
  this->packet = new Packet(buffer, releaseBuffer);

  D(cout.flush() << "--------------RequestOrResponse(buffer)\n";)

  // Kafka Protocol: int32 size - managed within the Packet class
}

RequestOrResponse::RequestOrResponse() : WireFormatter()
{
  this->packet = new Packet();

  D(cout.flush() << "--------------RequestOrResponse(params)\n";)
}

RequestOrResponse::~RequestOrResponse()
{
  delete this->packet;

  D(cout.flush() << "--------------~RequestOrResponse()\n";)
}

unsigned char* RequestOrResponse::toWireFormat(bool updatePacketSize)
{
  D(cout.flush() << "--------------RequestOrResponse::toWireFormat()\n";)

  // Kafka Protocol: int32 size - managed within the Packet class
  unsigned char *buffer = this->packet->toWireFormat(false);
  if (updatePacketSize) this->packet->updatePacketSize();
  return buffer;
}

int RequestOrResponse::getWireFormatSize(bool includePacketSize)
{
  D(cout.flush() << "--------------RequestOrResponse::getWireFormatSize()\n";)

  // Packet.size

  int size = 0;
  if (includePacketSize) size += this->packet->getWireFormatSize(includePacketSize);
  return size;
}

int RequestOrResponse::size(bool includeProtocolSizeFieldLength) const
{
  return this->packet->getSize(includeProtocolSizeFieldLength);
}

ostream& operator<< (ostream& os, const RequestOrResponse& r)
{
  os << "RequestOrResponse.size:" << r.size() << "\n";
  return os;
}

}; // namespace LibKafka
