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

#include "ProduceResponsePartition.h"
#include "../ApiConstants.h"

using namespace std;

namespace LibKafka {

ProduceResponsePartition::ProduceResponsePartition(Packet *packet) : WireFormatter(), PacketWriter(packet)
{
  D(cout.flush() << "--------------ProduceResponsePartition(buffer)\n";)

  // Kafka Protocol: int partition
  this->partition = this->packet->readInt32();

  // Kafka Protocol: short int errorCode
  this->errorCode = this->packet->readInt16();

  // Kafka Protocol: long int offset
  this->offset = this->packet->readInt64();
}

ProduceResponsePartition::ProduceResponsePartition(int partition, short int errorCode, long int offset) : WireFormatter(), PacketWriter()
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

ostream& operator<< (ostream& os, const ProduceResponsePartition& pm)
{
  os << pm.partition << ":" << pm.errorCode << ":" << pm.offset << ":" << ApiConstants::getErrorString(pm.errorCode) << "\n";
  return os;
}

}; // namespace LibKafka
