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

#include "Message.h"

using namespace std;

namespace LibKafka {

Message::Message(Packet *packet, long int offset) : WireFormatter(), PacketWriter(packet)
{
  D(cout.flush() << "--------------Message(buffer)\n";)

  // parsed and set by the MessageSet buffer parser
  this->offset = offset;

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

Message::Message(int crc, unsigned char magicByte, unsigned char attributes, int keyLength, unsigned char* key, int valueLength, unsigned char* value, long int offset, bool releaseArrays) : WireFormatter(), PacketWriter()
{
  D(cout.flush() << "--------------Message(params)\n";)

  this->crc = crc;
  this->magicByte = magicByte;
  this->attributes = attributes;
  this->keyLength = keyLength;
  this->key = key;
  this->valueLength = valueLength;
  this->value = value;
  this->offset = offset;
  this->releaseArrays = releaseArrays;
}

Message::~Message()
{
  D(cout.flush() << "--------------~Message():releaseArrays:" << this->releaseArrays << "\n";)

  if (this->releaseArrays)
  {
    D(cout.flush() << "--------------~Message():deleting key/value:" << this->key << ":" << this->value << "\n";)
    delete[] this->key;
    delete[] this->value;
  }
}

unsigned char* Message::toWireFormat(bool updatePacketSize)
{
  D(cout.flush() << "--------------Message::toWireFormat()\n";)
  
  // Kafka Protocol: int crc (see beginCRC32()/endCRC32() semantics in Packet.cc)
  // crc defined in the protocol as CRC for remaining bytes in message
  this->packet->beginCRC32();

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

  // calculate and update crc field (see beginCRC32()/endCRC32() semantics in Packet.cc)
  this->crc = this->packet->endCRC32();
  
  if (updatePacketSize) this->packet->updatePacketSize();
  return this->packet->getBuffer();
}

int Message::getWireFormatSize(bool includePacketSize)
{
  D(cout.flush() << "--------------Message::getWireFormatSize()\n";)
  
  // Packet.size
  // crc + magicByte + attributes
  // sizeof(keyLength) + keyLength
  // sizeof(valueLength) + valueLength

  int size = 0;
  if (includePacketSize) size += sizeof(int);
  size += sizeof(int) + sizeof(signed char) + sizeof(signed char);
  size += sizeof(int) + this->keyLength;
  size += sizeof(int) + this->valueLength;
  return size;
}

ostream& operator<< (ostream& os, const Message& m)
{
  os << "Message.offset(from MessageSet):" << m.offset << "\n";
  os << "Message.crc:" << m.crc << "\n";
  os << "Message.magicByte:" << m.magicByte << "\n";
  os << "Message.attributes:" << m.attributes << "\n";
  os << "Message.keyLength:" << m.keyLength << "\n";
  os << "Message.valueLength:" << m.valueLength << "\n";
  return os;
}

}; // namespace LibKafka
