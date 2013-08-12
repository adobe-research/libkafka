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

RequestOrResponse::RequestOrResponse(unsigned char *buffer)
{
  packet = new Packet(buffer);

  D(cout << "--------------RequestOrResponse(buffer)\n";)

  // Kafka Protocol: int32 size - managed within the Packet class
}

RequestOrResponse::RequestOrResponse()
{
  packet = new Packet();

  D(cout << "--------------RequestOrResponse(params)\n";)
}

unsigned char* RequestOrResponse::toWireFormat(bool updateSize)
{
  D(cout << "--------------RequestOrResponse::toWireFormat()\n";)

  // Kafka Protocol: int32 size - managed within the Packet class
  unsigned char *buffer = this->packet->toWireFormat(false);
  if (updateSize) this->packet->updatePacketSize();
  return buffer;
}

int RequestOrResponse::size()
{
  return this->packet->size;
}
