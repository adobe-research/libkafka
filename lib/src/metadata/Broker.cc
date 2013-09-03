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

#include <Broker.h>

using namespace std;

namespace LibKafka {

Broker::Broker(Packet *packet) : WireFormatter(), PacketWriter(packet)
{
  D(cout.flush() << "--------------Broker(buffer)\n";)

  // Kafka Protocol: int nodeId
  this->nodeId = this->packet->readInt32();

  // Kafka Protocol: kafka string host
  this->host = this->packet->readString();

  // Kafka Protocol: int port
  this->port = this->packet->readInt32();
}

Broker::Broker(int nodeId, string host, int port) : WireFormatter(), PacketWriter()
{
  D(cout.flush() << "--------------Broker(params)\n";)

  this->nodeId = nodeId;
  this->host = host;
  this->port = port;
}

unsigned char* Broker::toWireFormat(bool updatePacketSize)
{
  D(cout.flush() << "--------------Broker::toWireFormat()\n";)

  // Kafka Protocol: int nodeId
  this->packet->writeInt32(this->nodeId);

  // Kafka Protocol: kafka string host
  this->packet->writeString(this->host);

  // Kafka Protocol: int port
  this->packet->writeInt32(this->port);

  if (updatePacketSize) this->packet->updatePacketSize();
  return this->packet->getBuffer();
}

int Broker::getWireFormatSize(bool includePacketSize)
{
  D(cout.flush() << "--------------Broker::getWireFormatSize()\n";)

  // Packet.size
  // nodeId + host + port

  int size = 0;
  if (includePacketSize) size += sizeof(int);
  size += sizeof(int) + sizeof(short int) + this->host.length() + sizeof(int);
  return size;
}

ostream& operator<< (ostream& os, const Broker& b)
{
  os << b.nodeId << ":" << b.host << ":" << b.port;
  return os;
}

}; // namespace LibKafka
