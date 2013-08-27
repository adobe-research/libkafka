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

#include "FetchPartition.h"
#include "../ApiConstants.h"
#include "../Util.h"

using namespace std;

namespace LibKafka {

  FetchPartition::FetchPartition(Packet *packet) : WireFormatter(), PacketWriter(packet)
  {
    D(cout.flush() << "--------------FetchPartition(buffer)\n";)

    // Kafka Protocol: int partition
    this->partition = this->packet->readInt32();

    // Kafka Protocol: int fetchOffset
    this->fetchOffset = this->packet->readInt64();
    
    // Kafka Protocol: int maxBytes
    this->maxBytes = this->packet->readInt32();
  }

  FetchPartition::FetchPartition(int partition, long int fetchOffset, int maxBytes) : WireFormatter(), PacketWriter()
  {
    D(cout.flush() << "--------------FetchPartition(params)\n";)

    this->partition = partition;
    this->fetchOffset = fetchOffset;
    this->maxBytes = maxBytes;
  }

  FetchPartition::~FetchPartition()
  {
  }

  unsigned char* FetchPartition::toWireFormat(bool updatePacketSize)
  {
    D(cout.flush() << "--------------FetchPartition::toWireFormat()\n";)

    // Kafka Protocol: int partition
    this->packet->writeInt32(this->partition);

    // Kafka Protocol: int fetchOffset
    this->packet->writeInt64(this->fetchOffset);

    // Kafka Protocol: int maxBytes
    this->packet->writeInt32(this->maxBytes);

    if (updatePacketSize) this->packet->updatePacketSize();
    return this->packet->getBuffer();
  }

  int FetchPartition::getWireFormatSize(bool includePacketSize)
  {
    D(cout.flush() << "--------------FetchPartition::getWireFormatSize()\n";)

    // Packet.size
    // partition + fetchOffset + maxBytes

    int size = 0;
    if (includePacketSize) size += sizeof(int);
    size += sizeof(int) + sizeof(long int) + sizeof(int);
    return size;
  }

  ostream& operator<< (ostream& os, const FetchPartition& pm)
  {
    os << "FetchPartition.partition:" << pm.partition << "\n";
    os << "FetchPartition.fetchOffset:" << pm.fetchOffset << "\n";
    os << "FetchPartition.maxBytes:" << pm.maxBytes << "\n";
    return os;
  }

}; // namespace LibKafka
