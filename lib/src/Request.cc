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

#include <Request.h>

using namespace std;

Request::Request(unsigned char *buffer) : RequestOrResponse(buffer)
{
  D(cout << "--------------Request(buffer)\n";)

  // Kafka Protocol: short int apiKey
  this->apiKey = this->packet->read_int16();

  // Kafka Protocol: short int apiVersion
  this->apiVersion = this->packet->read_int16();

  // Kafka Protocol: int correlationId
  this->correlationId = this->packet->read_int32();

  // Kafka Protocol: kafka string clientId
  this->clientId = this->packet->read_string();
}

Request::Request(short int apiKey, short int apiVersion, int correlationId, string clientId) : RequestOrResponse()
{
  D(cout << "--------------Request(params)\n";)

  this->apiKey = apiKey;
  this->apiVersion = apiVersion;
  this->correlationId = correlationId;
  this->clientId = clientId;
}

unsigned char* Request::toWireFormat()
{
  unsigned char* buffer = this->RequestOrResponse::toWireFormat();

  D(cout << "--------------Request::toWireFormat()\n";)

  // Kafka Protocol: short int apiKey
  this->packet->write_int16(this->apiKey);

  // Kafka Protocol: short int apiVersion
  this->packet->write_int16(this->apiVersion);

  // Kafka Protocol: int correlationId
  this->packet->write_int32(this->correlationId);

  // Kafka Protocol: kafka string clientId
  this->packet->write_string(this->clientId);

  this->packet->update_packet_size();
  return buffer;
}
