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

#include <ProduceResponseTopic.h>

using namespace std;

namespace LibKafka {

  ProduceResponseTopic::ProduceResponseTopic(Packet *packet) : WireFormatter(), PacketWriter(packet)
  {
    D(cout.flush() << "--------------ProduceResponseTopic(buffer)\n";)

      // Kafka Protocol: kafka string topicName
      this->topicName = this->packet->readString();

    // Kafka Protocol: ProduceResponsePartition[]
    this->produceResponsePartitionArraySize = this->packet->readInt32();
    this->produceResponsePartitionArray = new ProduceResponsePartition*[this->produceResponsePartitionArraySize];
    for (int i=0; i<this->produceResponsePartitionArraySize; i++) {
      this->produceResponsePartitionArray[i] = new ProduceResponsePartition(this->packet);
    }

    this->releaseArrays = true;
  }

  ProduceResponseTopic::ProduceResponseTopic(string topicName, int produceResponsePartitionArraySize, ProduceResponsePartition **produceResponsePartitionArray, bool releaseArrays) : WireFormatter(), PacketWriter()
  {
    D(cout.flush() << "--------------ProduceResponseTopic(params)\n";)

      this->topicName = topicName;
    this->produceResponsePartitionArraySize = produceResponsePartitionArraySize;
    this->produceResponsePartitionArray = produceResponsePartitionArray;
    this->releaseArrays = releaseArrays;
  }

  ProduceResponseTopic::~ProduceResponseTopic()
  {
    if (this->releaseArrays)
    {
      for (int i=0; i<this->produceResponsePartitionArraySize; i++) {
	delete this->produceResponsePartitionArray[i];
      }
      delete[] this->produceResponsePartitionArray;
    }
  }

  unsigned char* ProduceResponseTopic::toWireFormat(bool updatePacketSize)
  {
    D(cout.flush() << "--------------ProduceResponseTopic::toWireFormat()\n";)

      // Kafka Protocol: kafka string topicName
      this->packet->writeString(this->topicName);

    // Kafka Protocol: ProduceResponsePartition[]
    this->packet->writeInt32(this->produceResponsePartitionArraySize);
    for (int i=0; i<this->produceResponsePartitionArraySize; i++) {
      this->produceResponsePartitionArray[i]->packet = this->packet;
      this->produceResponsePartitionArray[i]->toWireFormat(false);
    }

    if (updatePacketSize) this->packet->updatePacketSize();
    return this->packet->getBuffer();
  }

  int ProduceResponseTopic::getWireFormatSize(bool includePacketSize)
  {
    D(cout.flush() << "--------------ProduceResponseTopic::getWireFormatSize()\n";)

      // Packet.size
      // topicName
      // produceResponsePartitionArraySize*ProduceResponsePartition

      int size = 0;
    if (includePacketSize) size += sizeof(int);
    size += sizeof(short int) + this->topicName.length() + sizeof(int);
    for (int i=0; i<produceResponsePartitionArraySize; i++) {
      size += produceResponsePartitionArray[i]->getWireFormatSize(false);
    }
    return size;
  }

  ostream& operator<< (ostream& os, const ProduceResponseTopic& pt)
  {
    os << "ProduceResponse.topicName:" << pt.topicName << "\n";
    os << "ProduceResponse.produceResponsePartitionArraySize:" << pt.produceResponsePartitionArraySize << "\n";
    for (int i=0; i<pt.produceResponsePartitionArraySize; i++) {
      os << "ProduceResponse.produceResponsePartitionArray[" << i << "]:" << *(pt.produceResponsePartitionArray[i]) << "\n";
    }
    return os;
  }

}; // namespace LibKafka
