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

#include "FetchResponsePartition.h"
#include "../ApiConstants.h"

using namespace std;

namespace LibKafka {

FetchResponsePartition::FetchResponsePartition(Packet *packet) : WireFormatter(), PacketWriter(packet), ErrorHandler()
{
  D(cout.flush() << "--------------FetchResponsePartition(buffer)\n";)

  // Kafka Protocol: int partition
  this->partition = this->packet->readInt32();

  // Kafka Protocol: short int errorCode
  this->errorCode = this->packet->readInt16();

  // Kafka Protocol: long int highwaterMarkOffset
  this->highwaterMarkOffset = this->packet->readInt64();

  // Kafka Protocol: int messageSetSize
  this->messageSetSize = this->packet->readInt32();

  // Kafka Protocol: MessageSet messagesSet
  this->messageSet = new MessageSet(this->messageSetSize, this->packet);

  this->releaseArrays = true;
}

FetchResponsePartition::FetchResponsePartition(int partition, short int errorCode, long int highwaterMarkOffset, int messageSetSize, MessageSet *messageSet, bool releaseArrays) : WireFormatter(), PacketWriter(), ErrorHandler()
{
  D(cout.flush() << "--------------FetchResponsePartition(params)\n";)

  this->partition = partition;
  this->errorCode = errorCode;
  this->highwaterMarkOffset = highwaterMarkOffset;
  this->messageSetSize = messageSetSize;
  this->messageSet = messageSet;
  this->releaseArrays = releaseArrays;
}

FetchResponsePartition::~FetchResponsePartition()
{
  if (this->releaseArrays)
  {
    delete this->messageSet;
  }
}

unsigned char* FetchResponsePartition::toWireFormat(bool updatePacketSize)
{
  D(cout.flush() << "--------------FetchResponsePartition::toWireFormat()\n";)
  
  // Kafka Protocol: int partition
  this->packet->writeInt32(this->partition);

  // Kafka Protocol: short int errorCode
  this->packet->writeInt16(this->errorCode);

  // Kafka Protocol: long int highwaterMarkOffset
  this->packet->writeInt64(this->highwaterMarkOffset);
  
  // Kafka Protocol: int messageSetSize
  this->packet->writeInt32(this->messageSetSize);

  // Kafka Protocol: MessageSet messageSet
  this->messageSet->packet = this->packet;
  this->messageSet->toWireFormat(false);

  if (updatePacketSize) this->packet->updatePacketSize();
  return this->packet->getBuffer();
}

int FetchResponsePartition::getWireFormatSize(bool includePacketSize)
{
  D(cout.flush() << "--------------FetchResponsePartition::getWireFormatSize()\n";)
  
  // Packet.size
  // partition + errorCode + highwaterMarkOffset + messageSetSize + messageSet

  int size = 0;
  if (includePacketSize) size += sizeof(int);
  size += sizeof(int) + sizeof(short int) + sizeof(long int) + sizeof(int) + this->messageSetSize;
  return size;
}

bool FetchResponsePartition::hasErrorCode()
{
  return (this->errorCode != ApiConstants::ERRORCODE_NO_ERROR);
}

ostream& operator<< (ostream& os, const FetchResponsePartition& frp)
{
  os << "FetchResponsePartition:partition:" << frp.partition << "\n";
  os << "FetchResponsePartition:errorCode:" << frp.errorCode << ":" << ApiConstants::getErrorString(frp.errorCode) <<"\n";
  os << "FetchResponsePartition:highwaterMarkOffset:" << frp.highwaterMarkOffset << "\n";
  os << "FetchResponsePartition:messageSetSize:" << frp.messageSetSize << "\n";
  os << "FetchResponsePartition:MessageSet:\n" << *(frp.messageSet);
  return os;
}

}; // namespace LibKafka
