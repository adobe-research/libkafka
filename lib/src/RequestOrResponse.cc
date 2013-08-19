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

#include <RequestOrResponse.h>

using namespace std;

namespace LibKafka {

RequestOrResponse::RequestOrResponse(unsigned char *buffer, bool releaseBuffer) : WireFormatter()
{
  this->packet = new Packet(buffer, releaseBuffer);

  D(cout.flush() << "--------------RequestOrResponse(buffer)\n";)

  // Kafka Protocol: int32 size - managed within the Packet class
}

RequestOrResponse::RequestOrResponse() : WireFormatter()
{
  this->packet = new Packet();

  D(cout.flush() << "--------------RequestOrResponse(params)\n";)
}

RequestOrResponse::~RequestOrResponse()
{
  delete this->packet;

  D(cout.flush() << "--------------~RequestOrResponse()\n";)
}

unsigned char* RequestOrResponse::toWireFormat(bool updatePacketSize)
{
  D(cout.flush() << "--------------RequestOrResponse::toWireFormat()\n";)

  // Kafka Protocol: int32 size - managed within the Packet class
  unsigned char *buffer = this->packet->toWireFormat(false);
  if (updatePacketSize) this->packet->updatePacketSize();
  return buffer;
}

int RequestOrResponse::getWireFormatSize(bool includePacketSize)
{
  D(cout.flush() << "--------------RequestOrResponse::getWireFormatSize()\n";)

  // Packet.size

  int size = 0;
  if (includePacketSize) size += this->packet->getWireFormatSize(includePacketSize);
  return size;
}

int RequestOrResponse::size(bool includeProtocolSizeFieldLength) const
{
  return this->packet->getSize(includeProtocolSizeFieldLength);
}

ostream& operator<< (ostream& os, const RequestOrResponse& r)
{
  os << "RequestOrResponse.size:" << r.size() << "\n";
  return os;
}

}; // namespace LibKafka
