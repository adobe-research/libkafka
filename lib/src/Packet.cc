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
#include <arpa/inet.h>

#include <Packet.h>
#include <Util.h>

using namespace std;

namespace LibKafka {

const int Packet::DEFAULT_BUFFER_SIZE;

// Constructor to parse incoming Kafka protocol packet
Packet::Packet(unsigned char *buffer, bool releaseBuffer) : WireFormatter()
{
  D(cout.flush() << "--------------Packet(incoming)\n";)

  this->buffer = buffer;
  this->head = buffer;
  this->size = readInt32() + sizeof(int); // protocol size field is exclusive of size field length, size instance variable is inclusive of size field length
  this->releaseBuffer = releaseBuffer;
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

int Packet::getWireFormatSize(bool includeSize)
{
  D(cout.flush() << "--------------Packet::getWireFormatSize()\n";)

  // Packet.size

  int size = 0;
  if (includeSize) size += sizeof(int);
  return size;
}

Packet::~Packet()
{
  D(cout.flush() << "--------------~Packet()\n";)

  if (releaseBuffer) delete[] buffer;
}

unsigned char* Packet::toWireFormat(bool updateSize)
{
  D(cout.flush() << "--------------Packet::toWireFormat()\n";)

  if (updateSize) updatePacketSize();
  return this->buffer;
}

signed char Packet::readInt8()
{
  signed char netValue = *(signed char*)(this->head);
  // netValue == hostValue for single bytes
  this->head += sizeof(signed char);
  D(cout.flush() << "Packet::readInt8():netValue(" << netValue << "):hostValue(" << netValue << ")\n";)
  return netValue;
}

short int Packet::readInt16()
{
  short int netValue = *(int*)(this->head);
  short int hostValue = ntohs(netValue);
  this->head += sizeof(short int);
  D(cout.flush() << "Packet::readInt16():netValue(" << netValue << "):hostValue(" << hostValue << ")\n";)
  return hostValue;
}

int Packet::readInt32()
{
  int netValue = *(int*)(this->head);
  int hostValue = ntohl(netValue);
  this->head += sizeof(int);
  D(cout.flush() << "Packet::readInt32():netValue(" << netValue << "):hostValue(" << hostValue << ")\n";)
  return hostValue;
}

long int Packet::readInt64()
{
  long int netValue = *(long int*)(this->head);
  long int hostValue = ntohll(netValue);
  this->head += sizeof(long int);
  D(cout.flush() << "Packet::readInt64():netValue(" << netValue << "):hostValue(" << hostValue << ")\n";)
  return hostValue;
}

string Packet::readString()
{
  short int length = readInt16();
  string value = string((const char *)(this->head), length);
  this->head += length;
  D(cout.flush() << "Packet::readString():" << length << ":" << value << "\n";)
  return value;
}

unsigned char* Packet::readBytes(int numBytes)
{
  // returns a pointer to the bytes within the Packet buffer, and increments head
  unsigned char* bytes = this->head;
  this->head += numBytes;
  D(cout.flush() << "Packet::readBytes():" << numBytes << "\n";)
  return bytes;
}

void Packet::updatePacketSize()
{
  int netValue = htonl((this->size) - sizeof(int)); // prior to sending, set packetSize exclusive of size (int)
  memcpy(buffer, &netValue, sizeof(int));
  D(cout.flush() << "Packet::updatePacketSize():hostValue(" << this->size << "(:netValue(" << netValue << ")\n";)
}

void Packet::writeInt8(signed char hostValue)
{
  // netValue == hostValue for single bytes
  memcpy(head, &hostValue, sizeof(signed char));
  head += sizeof(signed char);
  this->size += sizeof(signed char);
  D(cout.flush() << "Packet::writeInt8():hostValue(" << hostValue << "):netValue(" << hostValue << ")\n";)
}

void Packet::writeInt16(short int hostValue)
{
  short int netValue = htons(hostValue);
  memcpy(head, &netValue, sizeof(short int));
  head += sizeof(short int);
  this->size += sizeof(short int);
  D(cout.flush() << "Packet::writeInt16():hostValue(" << hostValue << "):netValue(" << netValue << ")\n";)
}

void Packet::writeInt32(int hostValue)
{
  int netValue = htonl(hostValue);
  memcpy(head, &netValue, sizeof(int));
  head += sizeof(int);
  this->size += sizeof(int);
  D(cout.flush() << "Packet::writeInt32():hostValue(" << hostValue << "):netValue(" << netValue << ")\n";)
}

void Packet::writeInt64(long int hostValue)
{
  long int netValue = htonll(hostValue);
  memcpy(head, &netValue, sizeof(long int));
  head += sizeof(long int);
  this->size += sizeof(long int);
  D(cout.flush() << "Packet::writeInt64():Value(" << hostValue << "):netValue(" << netValue << ")\n";)
}

void Packet::writeString(string value)
{
  short int length = value.length();
  writeInt16(length);
  memcpy(head, value.c_str(), length);
  head += length;
  this->size += length;
  D(cout.flush() << "Packet::writeString():" << length << ":" << value.c_str() << "\n";)
}

void Packet::writeBytes(unsigned char* bytes, int numBytes)
{
  memcpy(head, bytes, numBytes);
  head += numBytes;
  this->size += numBytes;
  D(cout.flush() << "Packet::writeBytes():" << numBytes << "\n";)
}

void Packet::resetForReading()
{
  this->head = buffer;
  this->size = readInt32();
}

int Packet::getSize(bool includeProtocolSizeFieldLength)
{
  if (includeProtocolSizeFieldLength) return this->size; // size instance variable is inclusive of size field length
  return (this->size - sizeof(int)); // protocol size field is exclusive of size field length
}

}; // namespace LibKafka
