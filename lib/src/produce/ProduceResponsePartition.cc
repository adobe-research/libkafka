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

#include "ProduceResponsePartition.h"
#include "../ApiConstants.h"

using namespace std;

namespace LibKafka {

ProduceResponsePartition::ProduceResponsePartition(Packet *packet) : WireFormatter(), PacketWriter(packet), ErrorHandler()
{
  D(cout.flush() << "--------------ProduceResponsePartition(buffer)\n";)

  // Kafka Protocol: int partition
  this->partition = this->packet->readInt32();

  // Kafka Protocol: short int errorCode
  this->errorCode = this->packet->readInt16();

  // Kafka Protocol: long int offset
  this->offset = this->packet->readInt64();
}

ProduceResponsePartition::ProduceResponsePartition(int partition, short int errorCode, long int offset) : WireFormatter(), PacketWriter(), ErrorHandler()
{
  D(cout.flush() << "--------------ProduceResponsePartition(params)\n";)

  this->partition = partition;
  this->errorCode = errorCode;
  this->offset = offset;
}

ProduceResponsePartition::~ProduceResponsePartition()
{
}

unsigned char* ProduceResponsePartition::toWireFormat(bool updatePacketSize)
{
  D(cout.flush() << "--------------ProduceResponsePartition::toWireFormat()\n";)
  
  // Kafka Protocol: int partition
  this->packet->writeInt32(this->partition);

  // Kafka Protocol: short int errorCode
  this->packet->writeInt16(this->errorCode);

  // Kafka Protocol: long int offset
  this->packet->writeInt64(this->offset);

  if (updatePacketSize) this->packet->updatePacketSize();
  return this->packet->getBuffer();
}

int ProduceResponsePartition::getWireFormatSize(bool includePacketSize)
{
  D(cout.flush() << "--------------ProduceResponsePartition::getWireFormatSize()\n";)
  
  // Packet.size
  // partition + errorCode + offset

  int size = 0;
  if (includePacketSize) size += sizeof(int);
  size += sizeof(int) + sizeof(short int) + sizeof(long int);
  return size;
}

bool ProduceResponsePartition::hasErrorCode()
{
  return (this->errorCode != ApiConstants::ERRORCODE_NO_ERROR);
}

ostream& operator<< (ostream& os, const ProduceResponsePartition& pm)
{
  os << pm.partition << ":" << pm.errorCode << ":" << pm.offset << ":" << ApiConstants::getErrorString(pm.errorCode) << "\n";
  return os;
}

}; // namespace LibKafka
