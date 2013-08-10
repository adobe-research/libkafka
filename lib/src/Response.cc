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

Response::Response(unsigned char *buffer) : RequestOrResponse(buffer)
{
  D(cout << "--------------Response(buffer)\n";)
 
  // Kafka Protocol: int correlationId
  this->correlationId = this->packet->read_int32();
}

Response::Response(int correlationId) : RequestOrResponse()
{
  D(cout << "--------------Response(params)\n";)
 
  // Kafka Protocol: int correlationId
  this->correlationId = correlationId;
}

unsigned char* Response::toWireFormat()
{
  unsigned char* buffer = this->RequestOrResponse::toWireFormat();

  D(cout << "--------------Response::toWireFormat()\n";)

  // Kafka Protocol: int correlationId
  this->packet->write_int32(this->correlationId);

  this->packet->update_packet_size();
  return buffer;
}
