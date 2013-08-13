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

#include <Broker.h>

using namespace std;

Broker::Broker(Packet *packet) : WireFormatter(), PacketWriter(packet)
{
  D(cout << "--------------Broker(buffer)\n";)

    // Kafka Protocol: int nodeId
    this->nodeId = this->packet->readInt32();

  // Kafka Protocol: kafka string host
  this->host = this->packet->readString();

  // Kafka Protocol: int port
  this->port = this->packet->readInt32();
}

Broker::Broker(int nodeId, string host, int port) : WireFormatter(), PacketWriter()
{
  D(cout << "--------------Broker(params)\n";)

    this->nodeId = nodeId;
  this->host = host;
  this->port = port;
}

unsigned char* Broker::toWireFormat(bool updateSize)
{
  D(cout << "--------------Broker::toWireFormat()\n";)

  // Kafka Protocol: int nodeId
  this->packet->writeInt32(this->nodeId);

  // Kafka Protocol: kafka string host
  this->packet->writeString(this->host);

  // Kafka Protocol: int port
  this->packet->writeInt32(this->port);

  if (updateSize) this->packet->updatePacketSize();
  return this->packet->getBuffer();
}

ostream& operator<< (ostream& os, const Broker& b)
{
  os << b.nodeId << ":" << b.host << ":" << b.port << "\n";
  return os;
}
