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

#include "OffsetResponsePartition.h"
#include "../ApiConstants.h"
#include "../Util.h"

using namespace std;

namespace LibKafka {

OffsetResponsePartition::OffsetResponsePartition(Packet *packet) : WireFormatter(), PacketWriter(packet), ErrorHandler()
{
  D(cout.flush() << "--------------OffsetResponsePartition(buffer)\n";)

  // Kafka Protocol: int partition
  this->partition = this->packet->readInt32();

  // Kafka Protocol: short int errorCode
  this->errorCode = this->packet->readInt16();

  // Kafka Protocol: long int offset[]
  this->offsetArraySize = this->packet->readInt32();
  this->offsetArray = new long int[this->offsetArraySize];
  for (int i=0; i<this->offsetArraySize; i++) {
    this->offsetArray[i] = this->packet->readInt64();
  }

  this->releaseArrays = true;
}

OffsetResponsePartition::OffsetResponsePartition(int partition, short int errorCode, int offsetArraySize, long int *offsetArray, bool releaseArrays) : WireFormatter(), PacketWriter(), ErrorHandler()
{
  D(cout.flush() << "--------------OffsetResponsePartition(params)\n";)

  this->partition = partition;
  this->errorCode = errorCode;
  this->offsetArraySize = offsetArraySize;
  this->offsetArray = offsetArray;
  this->releaseArrays = releaseArrays;
}

OffsetResponsePartition::~OffsetResponsePartition()
{
  if (this->releaseArrays)
  {
    delete[] this->offsetArray;
  }
}

unsigned char* OffsetResponsePartition::toWireFormat(bool updatePacketSize)
{
  D(cout.flush() << "--------------OffsetResponsePartition::toWireFormat()\n";)
  
  // Kafka Protocol: int partition
  this->packet->writeInt32(this->partition);

  // Kafka Protocol: short int errorCode
  this->packet->writeInt16(this->errorCode);

  // Kafka Protocol: long int offset[]
  this->packet->writeInt32(this->offsetArraySize);
  for (int i=0; i<this->offsetArraySize; i++) {
    this->packet->writeInt64(this->offsetArray[i]);
  }

  if (updatePacketSize) this->packet->updatePacketSize();
  return this->packet->getBuffer();
}

int OffsetResponsePartition::getWireFormatSize(bool includePacketSize)
{
  D(cout.flush() << "--------------OffsetResponsePartition::getWireFormatSize()\n";)
  
  // Packet.size
  // partition + errorCode + offset[]

  int size = 0;
  if (includePacketSize) size += sizeof(int);
  size += sizeof(int) + sizeof(short int) + sizeof(int) + (this->offsetArraySize * sizeof(long int));
  return size;
}

bool OffsetResponsePartition::hasErrorCode()
{
  return (this->errorCode != ApiConstants::ERRORCODE_NO_ERROR);
}

ostream& operator<< (ostream& os, const OffsetResponsePartition& frp)
{
  os << "OffsetResponsePartition:partition:" << frp.partition << "\n";
  os << "OffsetResponsePartition:errorCode:" << frp.errorCode << ":" << ApiConstants::getErrorString(frp.errorCode) <<"\n";
  os << "OffsetResponsePartition:offsetArraySize" << frp.offsetArraySize << "\n";
  os << "OffsetResponsePartition:offsetArray" << longIntArrayToString(frp.offsetArray, frp.offsetArraySize) << "\n";
  return os;
}

}; // namespace LibKafka
