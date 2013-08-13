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

// Constructor to parse incoming Kafka protocol packet
Packet::Packet(unsigned char *buffer) : WireFormatter()
{
  D(cout.flush() << "--------------Packet(incoming)\n";)

  this->buffer = buffer;
  this->head = buffer;
  this->size = readInt32();
  this->releaseBuffer = false;
}

// Constructor to construct outgoing Kafka protocol packet
Packet::Packet(int bufferSize) : WireFormatter()
{
  D(cout.flush() << "--------------Packet(outgoing)\n";)
  
  buffer = new unsigned char[bufferSize];
  head = buffer;
  this->size = 0;
  writeInt32(this->size);

  this->releaseBuffer = true;
}

Packet::~Packet()
{
  D(cout.flush() << "--------------~Packet()\n";)

  if (releaseBuffer) delete buffer;
}

unsigned char* Packet::toWireFormat(bool updateSize)
{
  D(cout.flush() << "--------------Packet::toWireFormat()\n";)

  if (updateSize) updatePacketSize();
  return this->buffer;
}

short int Packet::readInt16()
{
  short int value = *(int*)(this->head);
  this->head += sizeof(short int);
  D(cout.flush() << "Packet::readInt16():" << value << "\n";)
  return value;
}

int Packet::readInt32()
{
  int value = *(int*)(this->head);
  this->head += sizeof(int);
  D(cout.flush() << "Packet::readInt32():" << value << "\n";)
  return value;
}

long int Packet::readInt64()
{
  long int value = *(long int*)(this->head);
  this->head += sizeof(long int);
  D(cout.flush() << "Packet::readInt64():" << value << "\n";)
  return value;
}

string Packet::readString()
{
  short int length = readInt16();
  string value = string((const char *)(this->head), length);
  this->head += length;
  D(cout.flush() << "Packet::readString():" << length << ":" << value << "\n";)
  return value;
}

void Packet::updatePacketSize()
{
  memcpy(buffer, &(this->size), sizeof(int));
  D(cout.flush() << "Packet::updatePacketSize():" << this->size << "\n";)
}

void Packet::writeInt16(short int value)
{
  memcpy(head, &value, sizeof(short int));
  head += sizeof(short int);
  this->size += sizeof(short int);
  D(cout.flush() << "Packet::writeInt16():" << value << "\n";)
}

void Packet::writeInt32(int value)
{
  memcpy(head, &value, sizeof(int));
  head += sizeof(int);
  this->size += sizeof(int);
  D(cout.flush() << "Packet::writeInt32():" << value << "\n";)
}

void Packet::writeInt64(long int value)
{
  memcpy(head, &value, sizeof(long int));
  head += sizeof(long int);
  this->size += sizeof(long int);
  D(cout.flush() << "Packet::writeInt64():" << value << "\n";)
}

void Packet::writeString(string value)
{
  short int length = value.length();
  memcpy(head, &length, sizeof(short int));
  head += sizeof(short int);
  this->size += sizeof(short int);
  memcpy(head, value.c_str(), length);
  head += length;
  this->size += length;
  D(cout.flush() << "Packet::writeString():" << length << ":" << value.c_str() << "\n";)
}

void Packet::resetForReading()
{
  this->head = buffer;
  this->size = readInt32();
}
