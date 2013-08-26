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

#include <ProduceTopic.h>
#include <ProduceMessageSet.h>

using namespace std;

namespace LibKafka {

ProduceTopic::ProduceTopic(Packet *packet) : WireFormatter(), PacketWriter(packet)
{
  D(cout.flush() << "--------------ProduceTopic(buffer)\n";)

  // Kafka Protocol: kafka string topicName
  this->topicName = this->packet->readString();

  // Kafka Protocol: ProduceMessageSet[]
  this->produceMessageSetArraySize = this->packet->readInt32();
  this->produceMessageSetArray = new ProduceMessageSet*[this->produceMessageSetArraySize];
  for (int i=0; i<this->produceMessageSetArraySize; i++) {
    this->produceMessageSetArray[i] = new ProduceMessageSet(this->packet);
  }
  
  this->releaseArrays = true;
}

ProduceTopic::ProduceTopic(string topicName, int produceMessageSetArraySize, ProduceMessageSet **produceMessageSetArray, bool releaseArrays) : WireFormatter(), PacketWriter()
{
  D(cout.flush() << "--------------ProduceTopic(params)\n";)

  this->topicName = topicName;
  this->produceMessageSetArraySize = produceMessageSetArraySize;
  this->produceMessageSetArray = produceMessageSetArray;
  this->releaseArrays = releaseArrays;
}

ProduceTopic::~ProduceTopic()
{
  if (this->releaseArrays)
  {
    for (int i=0; i<this->produceMessageSetArraySize; i++) {
      delete this->produceMessageSetArray[i];
    }
    delete[] this->produceMessageSetArray;
  }
}

unsigned char* ProduceTopic::toWireFormat(bool updatePacketSize)
{
  D(cout.flush() << "--------------ProduceTopic::toWireFormat()\n";)
  
  // Kafka Protocol: kafka string topicName
  this->packet->writeString(this->topicName);

  // Kafka Protocol: ProduceMessageSet[]
  this->packet->writeInt32(this->produceMessageSetArraySize);
  for (int i=0; i<this->produceMessageSetArraySize; i++) {
    this->produceMessageSetArray[i]->packet = this->packet;
    this->produceMessageSetArray[i]->toWireFormat(false);
  }

  if (updatePacketSize) this->packet->updatePacketSize();
  return this->packet->getBuffer();
}

int ProduceTopic::getWireFormatSize(bool includePacketSize)
{
  D(cout.flush() << "--------------ProduceTopic::getWireFormatSize()\n";)
  
  // Packet.size
  // topicName
  // partitionMetadataArraySize*PartitionMetadata

  int size = 0;
  if (includePacketSize) size += sizeof(int);
  size += sizeof(short int) + this->topicName.length() + sizeof(int);
  for (int i=0; i<produceMessageSetArraySize; i++) {
    size += produceMessageSetArray[i]->getWireFormatSize(false);
  }
  return size;
}

ostream& operator<< (ostream& os, const ProduceTopic& pt)
{
  os << pt.topicName << ":" << pt.produceMessageSetArraySize;
  return os;
}

}; // namespace LibKafka
