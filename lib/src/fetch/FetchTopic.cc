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

#include <FetchTopic.h>
#include <FetchPartition.h>

using namespace std;

namespace LibKafka {

FetchTopic::FetchTopic(Packet *packet) : WireFormatter(), PacketWriter(packet)
{
  D(cout.flush() << "--------------FetchTopic(buffer)\n";)

  // Kafka Protocol: kafka string topicName
  this->topicName = this->packet->readString();

  // Kafka Protocol: FetchPartition[]
  this->fetchPartitionArraySize = this->packet->readInt32();
  this->fetchPartitionArray = new FetchPartition*[this->fetchPartitionArraySize];
  for (int i=0; i<this->fetchPartitionArraySize; i++) {
    this->fetchPartitionArray[i] = new FetchPartition(this->packet);
  }
  
  this->releaseArrays = true;
}

FetchTopic::FetchTopic(string topicName, int fetchPartitionArraySize, FetchPartition **fetchPartitionArray, bool releaseArrays) : WireFormatter(), PacketWriter()
{
  D(cout.flush() << "--------------FetchTopic(params)\n";)

  this->topicName = topicName;
  this->fetchPartitionArraySize = fetchPartitionArraySize;
  this->fetchPartitionArray = fetchPartitionArray;
  this->releaseArrays = releaseArrays;
}

FetchTopic::~FetchTopic()
{
  if (this->releaseArrays)
  {
    for (int i=0; i<this->fetchPartitionArraySize; i++) {
      delete this->fetchPartitionArray[i];
    }
    delete[] this->fetchPartitionArray;
  }
}

unsigned char* FetchTopic::toWireFormat(bool updatePacketSize)
{
  D(cout.flush() << "--------------FetchTopic::toWireFormat()\n";)
  
  // Kafka Protocol: kafka string topicName
  this->packet->writeString(this->topicName);

  // Kafka Protocol: FetchPartition[]
  this->packet->writeInt32(this->fetchPartitionArraySize);
  for (int i=0; i<this->fetchPartitionArraySize; i++) {
    this->fetchPartitionArray[i]->packet = this->packet;
    this->fetchPartitionArray[i]->toWireFormat(false);
  }

  if (updatePacketSize) this->packet->updatePacketSize();
  return this->packet->getBuffer();
}

int FetchTopic::getWireFormatSize(bool includePacketSize)
{
  D(cout.flush() << "--------------FetchTopic::getWireFormatSize()\n";)
  
  // Packet.size
  // topicName
  // fetchPartitionArraySize*FetchPartition

  int size = 0;
  if (includePacketSize) size += sizeof(int);
  size += sizeof(short int) + this->topicName.length() + sizeof(int);
  for (int i=0; i<fetchPartitionArraySize; i++) {
    size += fetchPartitionArray[i]->getWireFormatSize(false);
  }
  return size;
}

ostream& operator<< (ostream& os, const FetchTopic& pt)
{
  os << pt.topicName << ":" << pt.fetchPartitionArraySize;
  return os;
}

}; // namespace LibKafka
