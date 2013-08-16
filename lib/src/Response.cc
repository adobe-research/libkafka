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

#include <iostream>

#include <Response.h>

using namespace std;

Response::Response(unsigned char *buffer, bool releaseBuffer) : RequestOrResponse(buffer, releaseBuffer)
{
  D(cout.flush() << "--------------Response(buffer)\n";)
 
  // Kafka Protocol: int correlationId
  this->correlationId = this->packet->readInt32();
}

Response::Response(int correlationId) : RequestOrResponse()
{
  D(cout.flush() << "--------------Response(params)\n";)
 
  // Kafka Protocol: int correlationId
  this->correlationId = correlationId;
}

unsigned char* Response::toWireFormat(bool updatePacketSize)
{
  unsigned char* buffer = this->RequestOrResponse::toWireFormat(false);

  D(cout.flush() << "--------------Response::toWireFormat()\n";)

  // Kafka Protocol: int correlationId
  this->packet->writeInt32(this->correlationId);

  if (updatePacketSize) this->packet->updatePacketSize();
  return buffer;
}

int Response::getWireFormatSize(bool includePacketSize)
{
  D(cout.flush() << "--------------Response::getWireFormatSize()\n";)

  // RequestOrResponse.getWireFormatSize
  // correlationId

  int size = RequestOrResponse::getWireFormatSize(includePacketSize);
  size += sizeof(int);
  return size;
}

ostream& operator<< (ostream& os, const Response& r)
{
  os << (const RequestOrResponse&)r;
  os << "Response.correlationId:" << r.correlationId << "\n";
  return os;
}
