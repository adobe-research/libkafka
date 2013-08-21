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

#include <ProduceMessageSet.h>

using namespace std;

namespace LibKafka {

ProduceMessageSet::ProduceMessageSet(Packet *packet) : WireFormatter(), PacketWriter(packet)
{
  D(cout.flush() << "--------------ProduceMessageSet(buffer)\n";)

  // Kafka Protocol: int partition
  this->partition = this->packet->readInt32();

  // Kafka Protocol: int messageSetSize
  this->messageSetSize = this->packet->readInt32();

  // Kafka Protocol: Message message
  this->messageSet = new MessageSet(this->packet);

  this->releaseArrays = true;
}

ProduceMessageSet::ProduceMessageSet(int partition, int messageSetSize, MessageSet *messageSet, bool releaseArrays) : WireFormatter(), PacketWriter()
{
  D(cout.flush() << "--------------ProduceMessageSet(params)\n";)

  this->partition = partition;
  this->messageSetSize = messageSetSize;
  this->messageSet = messageSet;
  this->releaseArrays = releaseArrays;
}

ProduceMessageSet::~ProduceMessageSet()
{
  if (this->releaseArrays)
  {
    delete this->messageSet;
  }
}

unsigned char* ProduceMessageSet::toWireFormat(bool updatePacketSize)
{
  D(cout.flush() << "--------------ProduceMessageSet::toWireFormat()\n";)
  
  // Kafka Protocol: int partition
  this->packet->writeInt32(this->partition);

  // Kafka Protocol: int messageSetSize
  this->packet->writeInt32(this->messageSetSize);

  // Kafka Protocol: MessageSet message
  this->messageSet->packet = this->packet;
  this->messageSet->toWireFormat(false);

  if (updatePacketSize) this->packet->updatePacketSize();
  return this->packet->getBuffer();
}

int ProduceMessageSet::getWireFormatSize(bool includePacketSize)
{
  D(cout.flush() << "--------------ProduceMessageSet::getWireFormatSize()\n";)
  
  // Packet.size
  // partition + messageSetSize + messageSet

  int size = 0;
  if (includePacketSize) size += sizeof(int);
  size += sizeof(int) + sizeof(int) + this->messageSetSize;
  return size;
}

ostream& operator<< (ostream& os, const ProduceMessageSet& pm)
{
  os << pm.partition << ":" << pm.messageSetSize << "\n";
  return os;
}

}; // namespace LibKafka
