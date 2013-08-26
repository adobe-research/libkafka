//////////////////////////////////////////////////////////////////
//                                                              //
// libkafka - C/C++ client for Apache Kafka v0.8+               //
//                                                              //
// David Tompkins -- 8/8/2013                                   //
// http://dt.org/                                               //
//                                                              //
// Copyright (c) 2013 by David Tompkins.                        //
//                                                              //
//////////////////////////////////////////////////////////////////
//                                                              //
// This program is free software; you can redistribute it       //
// and/or modify it under the terms of the GNU General Public   //
// License as published by the Free Software Foundation.        //
//                                                              //
// This program is distributed in the hope that it will be      //
// useful, but WITHOUT ANY WARRANTY; without even the implied   //
// warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR      //
// PURPOSE. See the GNU General Public License for more details //
//                                                              //
// You should have received a copy of the GNU General Public    //
// License along with this program; if not, write to the Free   //
// Software Foundation, Inc., 59 Temple Place, Suite 330,       //
// Boston, MA 02111-1307 USA                                    //
//                                                              //
//////////////////////////////////////////////////////////////////

#include <string>
#include <iostream>

#include "TopicMetadata.h"
#include "PartitionMetadata.h"
#include "../ApiConstants.h"

using namespace std;

namespace LibKafka {

TopicMetadata::TopicMetadata(Packet *packet) : WireFormatter(), PacketWriter(packet)
{
  D(cout.flush() << "--------------TopicMetadata(buffer)\n";)

  // Kafka Protocol: short int topicErrorCode
  this->topicErrorCode = this->packet->readInt16();

  // Kafka Protocol: kafka string topicName
  this->topicName = this->packet->readString();

  // Kafka Protocol: PartitionMetadata[]
  this->partitionMetadataArraySize = this->packet->readInt32();
  this->partitionMetadataArray = new PartitionMetadata*[this->partitionMetadataArraySize];
  for (int i=0; i<this->partitionMetadataArraySize; i++) {
    this->partitionMetadataArray[i] = new PartitionMetadata(this->packet);
  }
  
  this->releaseArrays = true;
}

TopicMetadata::TopicMetadata(short int topicErrorCode, string topicName, int partitionMetadataArraySize, PartitionMetadata **partitionMetadataArray, bool releaseArrays) : WireFormatter(), PacketWriter()
{
  D(cout.flush() << "--------------TopicMetadata(params)\n";)

  this->topicErrorCode = topicErrorCode;
  this->topicName = topicName;
  this->partitionMetadataArraySize = partitionMetadataArraySize;
  this->partitionMetadataArray = partitionMetadataArray;
  this->releaseArrays = releaseArrays;
}

TopicMetadata::~TopicMetadata()
{
  if (this->releaseArrays)
  {
    for (int i=0; i<this->partitionMetadataArraySize; i++) {
      delete this->partitionMetadataArray[i];
    }
    delete[] this->partitionMetadataArray;
  }
}

unsigned char* TopicMetadata::toWireFormat(bool updatePacketSize)
{
  D(cout.flush() << "--------------TopicMetadata::toWireFormat()\n";)
  
  // Kafka Protocol: short int topicErrorCode
  this->packet->writeInt16(this->topicErrorCode);

  // Kafka Protocol: kafka string topicName
  this->packet->writeString(this->topicName);

  // Kafka Protocol: PartitionMetadata[]
  this->packet->writeInt32(this->partitionMetadataArraySize);
  for (int i=0; i<this->partitionMetadataArraySize; i++) {
    this->partitionMetadataArray[i]->packet = this->packet;
    this->partitionMetadataArray[i]->toWireFormat(false);
  }

  if (updatePacketSize) this->packet->updatePacketSize();
  return this->packet->getBuffer();
}

int TopicMetadata::getWireFormatSize(bool includePacketSize)
{
  D(cout.flush() << "--------------TopicMetadata::getWireFormatSize()\n";)
  
  // Packet.size
  // topicErrorCode + topicName + partitionMetadataArraySize
  // partitionMetadataArraySize*PartitionMetadata

  int size = 0;
  if (includePacketSize) size += sizeof(int);
  size += sizeof(short int) + sizeof(short int) + this->topicName.length() + sizeof(int);
  for (int i=0; i<partitionMetadataArraySize; i++) {
    size += partitionMetadataArray[i]->getWireFormatSize(false);
  }
  return size;
}

ostream& operator<< (ostream& os, const TopicMetadata& tm)
{
  os << tm.topicErrorCode << ":" << ApiConstants::getErrorString(tm.topicErrorCode) << ":" << tm.topicName << ":" << tm.partitionMetadataArraySize;
  return os;
}

}; // namespace LibKafka
