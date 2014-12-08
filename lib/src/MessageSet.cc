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

#include "MessageSet.h"

using namespace std;

namespace LibKafka {

MessageSet::MessageSet(int messageSetSize, Packet *packet) : WireFormatter(), PacketWriter(packet)
{
  D(cout.flush() << "--------------MessageSet(buffer)\n";)

  // Read an arbitrary number of Messages (and possibly MessageSets) from the packet, based on size values

  this->messageSetSize = messageSetSize;
  int bytesRead = 0;

  while (bytesRead < this->messageSetSize)
  {
    int64_t offset = this->packet->readInt64();
    int messageSize = this->packet->readInt32();
    Message *message = new Message(packet, offset);
    int messageWireSize = message->getWireFormatSize(false);
    if (messageSize != messageWireSize)
    {
      // TODO: Nested MessageSet within message body, currently dropping the nested content
      E("MessageSet:nested MessageSet detected within message body:unimplemented\n");
      packet->seek(messageSize-messageWireSize);
    }
    this->messages.push_back(message);
    // increment bytesRead for offset and messageSize fields, then messageSize
    bytesRead += sizeof(int64_t) + sizeof(int) + messageSize;
  }

  if (bytesRead != this->messageSetSize)
  {
    E("MessageSet:error: bytes read does not equal messageSetSize value\n");
  }

  this->releaseArrays = true;
}

MessageSet::MessageSet(int messageSetSize, vector<Message*> messages, bool releaseArrays) : WireFormatter(), PacketWriter()
{
  D(cout.flush() << "--------------MessageSet(params)\n";)

  this->messageSetSize = messageSetSize;
  this->messages = messages;
  this->releaseArrays = releaseArrays;
}

MessageSet::~MessageSet()
{
  D(cout.flush() << "--------------~MessageSet():releaseArrays:" << this->releaseArrays << "\n";)
  
  if (this->releaseArrays)
  {
    for(vector<Message*>::const_iterator message=this->messages.begin(); message!=this->messages.end(); message++) {
      D(cout.flush() << "--------------~MessageSet():deleting message:" << *message << "\n";)
      delete *message;
    }
  }
}

unsigned char* MessageSet::toWireFormat(bool updatePacketSize)
{
  D(cout.flush() << "--------------MessageSet::toWireFormat()\n";)
  
  for(vector<Message*>::const_iterator message=this->messages.begin(); message!=this->messages.end(); ++message)
  {
    // Kafka Protocol: int64_t offset
    this->packet->writeInt64((*message)->offset);

    // Kafka Protocol: int messageSize (allow for Message size changes due to compression)
    unsigned char *messageSizeField;
    if ((*message)->hasCompression()) messageSizeField = this->packet->getHead();
    this->packet->writeInt32((*message)->getWireFormatSize(false));
    (*message)->packet = this->packet;
    (*message)->toWireFormat(false);
    if ((*message)->hasCompression())
    {
      D(cout.flush() << "--------------MessageSet::toWireFormat():updating messageSize field for compression\n";)
      this->packet->updateInt32((*message)->getWireFormatSize(false), messageSizeField);
    }
  }

  if (updatePacketSize) this->packet->updatePacketSize();
  return this->packet->getBuffer();
}

int MessageSet::getWireFormatSize(bool includePacketSize)
{
  D(cout.flush() << "--------------MessageSet::getWireFormatSize()\n";)
  
  // Packet.size
  // (offset + messageSize + Message)[]

  int size = 0;
  if (includePacketSize) size += sizeof(int);

  for(vector<Message*>::const_iterator message=this->messages.begin(); message!=this->messages.end(); ++message)
  {
    size += sizeof(int64_t) + sizeof(int);
    size += (*message)->getWireFormatSize(false);
  }

  return size;
}

void MessageSet::setCompression(int codec)
{
  for(vector<Message*>::const_iterator message=this->messages.begin(); message!=this->messages.end(); ++message)
  {
    (*message)->setCompression(codec);
  }
}

ostream& operator<< (ostream& os, const MessageSet& m)
{
  os << "MessageSet.numMessages:" << m.messages.size() << "\n";
  D(for(vector<Message*>::const_iterator message=m.messages.begin(); message!=m.messages.end(); ++message) os << *(*message);)
  return os;
}

}; // namespace LibKafka
