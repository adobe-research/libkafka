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

#include <PartitionMetadata.h>

using namespace std;

PartitionMetadata::PartitionMetadata(Packet *packet) : WireFormatter(), PacketWriter(packet)
{
  D(cout.flush() << "--------------PartitionMetadata(buffer)\n";)

  // Kafka Protocol: short int partitionErrorCode
  this->partitionErrorCode = this->packet->readInt16();

  // Kafka Protocol: int partitionId
  this->partitionId = this->packet->readInt32();

  // Kafka Protocol: int leader
  this->leader = this->packet->readInt32();

  // Kafka Protocol: int[] replica
  this->replicaArraySize = this->packet->readInt32();
  this->replicaArray = new int[this->replicaArraySize];
  for (int i=0; i<this->replicaArraySize; i++) {
    this->replicaArray[i] = this->packet->readInt32();
  }

  // Kafka Protocol: int[] isr
  this->isrArraySize = this->packet->readInt32();
  this->isrArray = new int[this->isrArraySize];
  for (int i=0; i<this->isrArraySize; i++) {
    this->isrArray[i] = this->packet->readInt32();
  }

  this->releaseArrays = true;
}

PartitionMetadata::PartitionMetadata(short int partitionErrorCode, int partitionId, int leader, int replicaArraySize, int *replicaArray, int isrArraySize, int *isrArray) : WireFormatter(), PacketWriter()
{
  D(cout.flush() << "--------------PartitionMetadata(params)\n";)

  this->partitionErrorCode = partitionErrorCode;
  this->partitionId = partitionId;
  this->leader = leader;
  this->replicaArraySize = replicaArraySize;
  this->replicaArray = replicaArray;
  this->isrArraySize = isrArraySize;
  this->isrArray = isrArray;
  this->releaseArrays = false;
}

unsigned char* PartitionMetadata::toWireFormat(bool updatePacketSize)
{
  D(cout.flush() << "--------------PartitionMetadata::toWireFormat()\n";)
  
  // Kafka Protocol: short int partitionErrorCode
  this->packet->writeInt16(this->partitionErrorCode);

  // Kafka Protocol: int partitionId
  this->packet->writeInt32(this->partitionId);

  // Kafka Protocol: int leader
  this->packet->writeInt32(this->leader);

  // Kafka Protocol: int[] replica
  this->packet->writeInt32(this->replicaArraySize);
  for (int i=0; i<this->replicaArraySize; i++) {
    this->packet->writeInt32(this->replicaArray[i]);
  }

  // Kafka Protocol: int[] isr
  this->packet->writeInt32(this->isrArraySize);
  for (int i=0; i<this->isrArraySize; i++) {
    this->packet->writeInt32(this->isrArray[i]);
  }

  if (updatePacketSize) this->packet->updatePacketSize();
  return this->packet->getBuffer();
}

int PartitionMetadata::getWireFormatSize(bool includePacketSize)
{
  D(cout.flush() << "--------------PartitionMetadata::getWireFormatSize()\n";)
  
  // Packet.size
  // partitionErrorCode + partitionId + leader
  // replicaArraySize + replicaArraySize*int32
  // isrArraySize + isrArraySize*int32

  int size = 0;
  if (includePacketSize) size += sizeof(int);
  size += sizeof(short int) + sizeof(int) + sizeof(int);
  size += sizeof(int) + (replicaArraySize * sizeof(int));
  size += sizeof(int) + (isrArraySize * sizeof(int));
  return size;
}

ostream& operator<< (ostream& os, const PartitionMetadata& pm)
{
  os << pm.partitionErrorCode << ":" << pm.partitionId << ":" << pm.leader << ":" << pm.replicaArray << ":" << pm.isrArray << "\n";
  return os;
}
