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

#include "MessageSet.h"

using namespace std;

namespace LibKafka {

MessageSet::MessageSet(Packet *packet) : WireFormatter(), PacketWriter(packet)
{
  D(cout.flush() << "--------------MessageSet(buffer)\n";)

  // Kafka Protocol: long int offset
  this->offset = this->packet->readInt64();

  // Kafka Protocol: int messageSize
  this->messageSize = this->packet->readInt32();

  // Kafka Protocol: int crc
  this->crc = this->packet->readInt32();
  
  // Kafka Protocol: signed char magicByte
  this->magicByte = this->packet->readInt8();

  // Kafka Protocol: signed char attributes
  this->attributes = this->packet->readInt8();
  
  // Kafka Protocol: bytes key
  this->keyLength = this->packet->readInt32();
  this->key = this->packet->readBytes(this->keyLength);

  // Kafka Protocol: bytes value
  this->valueLength = this->packet->readInt32();
  this->value = this->packet->readBytes(this->valueLength);

  this->releaseArrays = false; // key and value point into the Packet buffer, not new memory
}

MessageSet::MessageSet(long int offset, int messageSize, int crc, unsigned char magicByte, unsigned char attributes, int keyLength, unsigned char* key, int valueLength, unsigned char* value, bool releaseArrays) : WireFormatter(), PacketWriter()
{
  D(cout.flush() << "--------------MessageSet(params)\n";)

  this->offset = offset;
  this->messageSize = messageSize;
  this->crc = crc;
  this->magicByte = magicByte;
  this->attributes = attributes;
  this->keyLength = keyLength;
  this->key = key;
  this->valueLength = valueLength;
  this->value = value;
  this->releaseArrays = releaseArrays;
}

MessageSet::~MessageSet()
{
  if (this->releaseArrays)
  {
    delete[] this->key;
    delete[] this->value;
  }
}

unsigned char* MessageSet::toWireFormat(bool updatePacketSize)
{
  D(cout.flush() << "--------------MessageSet::toWireFormat()\n";)
  
  // Kafka Protocol: long int offset
  this->packet->writeInt64(this->offset);

  // Kafka Protocol: int messageSize
  this->packet->writeInt32(this->messageSize);

  // Kafka Protocol: int crc
  this->packet->writeInt32(this->crc);

  // Kafka Protocol: signed char magicByte
  this->packet->writeInt8(this->magicByte);

  // Kafka Protocol: signed char attributes
  this->packet->writeInt8(this->attributes);

  // Kafka Protocol: bytes key
  this->packet->writeInt32(this->keyLength);
  this->packet->writeBytes(this->key, this->keyLength);

  // Kafka Protocol: bytes value
  this->packet->writeInt32(this->valueLength);
  this->packet->writeBytes(this->value, this->valueLength);
  
  if (updatePacketSize) this->packet->updatePacketSize();
  return this->packet->getBuffer();
}

int MessageSet::getWireFormatSize(bool includePacketSize)
{
  D(cout.flush() << "--------------MessageSet::getWireFormatSize()\n";)
  
  // Packet.size
  // offset + messageSize + crc + magicByte + attributes
  // sizeof(keyLength) + keyLength
  // sizeof(valueLength) + valueLength

  int size = 0;
  if (includePacketSize) size += sizeof(int);
  size += sizeof(long int) + sizeof(int) + sizeof(int) + sizeof(signed char) + sizeof(signed char);
  size += sizeof(int) + this->keyLength;
  size += sizeof(int) + this->valueLength;
  return size;
}

ostream& operator<< (ostream& os, const MessageSet& m)
{
  os << m.offset << ":" << m.messageSize << ":" << m.crc << ":" << m.magicByte << ":" << m.attributes << ":" << m.keyLength << ":" << m.valueLength;
  return os;
}

}; // namespace LibKafka
