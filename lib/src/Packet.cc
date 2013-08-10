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

#include <Packet.h>

using namespace std;

Packet::Packet(unsigned char *buffer)
{
  D(cout << "--------------Packet(buffer)\n";)

  this->buffer = buffer;
  this->head = buffer;
  this->size = read_int32();
}

Packet::Packet()
{
  D(cout << "--------------Packet(params)\n";)
}

unsigned char* Packet::toWireFormat()
{
  D(cout << "--------------Packet::toWireFormat()\n";)

  buffer = new unsigned char[DEFAULT_BUFFER_SIZE];
  head = buffer;
  this->size = 0;

  // Kafka Protocol: int32 size
  write_int32(this->size);

  update_packet_size();
  return this->buffer;
}

short int Packet::read_int16()
{
  short int value = *(int*)(this->head);
  this->head += sizeof(short int);
  D(cout << "Packet::read_int16():" << value << "\n";)
  return value;
}

int Packet::read_int32()
{
  int value = *(int*)(this->head);
  this->head += sizeof(int);
  D(cout << "Packet::read_int32():" << value << "\n";)
  return value;
}

long int Packet::read_int64()
{
  long int value = *(long int*)(this->head);
  this->head += sizeof(long int);
  D(cout << "Packet::read_int64():" << value << "\n";)
  return value;
}

string Packet::read_string()
{
  short int length = read_int16();
  string value = string((const char *)(this->head), length);
  this->head += length;
  D(cout << "Packet::read_string():" << length << ":" << value << "\n";)
  return value;
}

void Packet::update_packet_size()
{
  memcpy(buffer, &(this->size), sizeof(int));
  D(cout << "Packet::update_packet_size():" << this->size << "\n";)
}

void Packet::write_int16(short int value)
{
  memcpy(head, &value, sizeof(short int));
  head += sizeof(short int);
  this->size += sizeof(short int);
  D(cout << "Packet::write_int16():" << value << "\n";)
}

void Packet::write_int32(int value)
{
  memcpy(head, &value, sizeof(int));
  head += sizeof(int);
  this->size += sizeof(int);
  D(cout << "Packet::write_int32():" << value << "\n";)
}

void Packet::write_int64(long int value)
{
  memcpy(head, &value, sizeof(long int));
  head += sizeof(long int);
  this->size += sizeof(long int);
  D(cout << "Packet::write_int64():" << value << "\n";)
}

void Packet::write_string(string value)
{
  short int length = value.length();
  memcpy(head, &length, sizeof(short int));
  head += sizeof(short int);
  this->size += sizeof(short int);
  memcpy(head, value.c_str(), length);
  head += length;
  this->size += length;
  D(cout << "Packet::write_string():" << length << ":" << value.c_str() << "\n";)
}
