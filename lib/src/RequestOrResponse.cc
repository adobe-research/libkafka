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
  D(cout << "--------------RequestOrResponse(buffer)\n";)

  this->buffer = buffer;
  this->head = buffer;

  // Kafka Protocol: int32 size
  this->size = read_int32();
}

RequestOrResponse::RequestOrResponse()
{
  D(cout << "--------------RequestOrResponse(params)\n";)
}

short int RequestOrResponse::read_int16()
{
  short int value = *(int*)(this->head);
  this->head += sizeof(short int);
  D(cout << "read_int16():" << value << "\n";)
  return value;
}

int RequestOrResponse::read_int32()
{
  int value = *(int*)(this->head);
  this->head += sizeof(int);
  D(cout << "read_int32():" << value << "\n";)
  return value;
}

long int RequestOrResponse::read_int64()
{
  long int value = *(long int*)(this->head);
  this->head += sizeof(long int);
  D(cout << "read_int64():" << value << "\n";)
  return value;
}

string RequestOrResponse::read_string()
{
  short int length = read_int16();
  string value = string((const char *)(this->head), length);
  this->head += length;
  D(cout << "read_string():" << length << ":" << value << "\n";)
  return value;
}

unsigned char* RequestOrResponse::toWireFormat()
{
  D(cout << "--------------RequestOrResponse::toWireFormat()\n";)

  buffer = new unsigned char[DEFAULT_BUFFER_SIZE];
  head = buffer;
  this->size = 0;

  // Kafka Protocol: int32 size
  write_int32(this->size);

  write_size();
  return this->buffer;
}

void RequestOrResponse::write_size()
{
  memcpy(buffer, &(this->size), sizeof(int));
  D(cout << "write_size():" << this->size << "\n";)
}

void RequestOrResponse::write_int16(short int value)
{
  memcpy(head, &value, sizeof(short int));
  head += sizeof(short int);
  this->size += sizeof(short int);
  D(cout << "write_int16():" << value << "\n";)
}

void RequestOrResponse::write_int32(int value)
{
  memcpy(head, &value, sizeof(int));
  head += sizeof(int);
  this->size += sizeof(int);
  D(cout << "write_int32():" << value << "\n";)
}

void RequestOrResponse::write_int64(long int value)
{
  memcpy(head, &value, sizeof(long int));
  head += sizeof(long int);
  this->size += sizeof(long int);
  D(cout << "write_int64():" << value << "\n";)
}

void RequestOrResponse::write_string(string value)
{
  short int length = value.length();
  memcpy(head, &length, sizeof(short int));
  head += sizeof(short int);
  this->size += sizeof(short int);
  memcpy(head, value.c_str(), length);
  head += length;
  this->size += length;
  D(cout << "write_string():" << length << ":" << value.c_str() << "\n";)
}
