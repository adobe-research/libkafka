///////////////////////////////////////////////////////////////////////////
//
// libkafka - C/C++ client for Apache Kafka v0.8+
//
// David Tompkins -- 8/8/2013
// tompkins@adobe_dot_com
//
///////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2013 Adobe Systems Incorporated. All rights reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
///////////////////////////////////////////////////////////////////////////

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

  bool compression = ((this->attributes & COMPRESSION_MASK) != 0);
  if (compression) { D(cout.flush() << "Message():compression enabled (unimplemented):" << (this->attributes & COMPRESSION_MASK) << "\n";) }
  
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
