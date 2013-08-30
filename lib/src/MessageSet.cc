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

MessageSet::MessageSet(int messageSetSize, Packet *packet) : WireFormatter(), PacketWriter(packet)
{
  D(cout.flush() << "--------------MessageSet(buffer)\n";)

  // Read an arbitrary number of Messages (and possibly MessageSets) from the packet, based on size values

  this->messageSetSize = messageSetSize;
  int bytesRead = 0;

  while (bytesRead < this->messageSetSize)
  {
    long int offset = this->packet->readInt64();
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
    bytesRead += sizeof(long int) + sizeof(int) + messageSize;
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
    // Kafka Protocol: long int offset
    this->packet->writeInt64((*message)->offset);

    // Kafka Protocol: int messageSize
    this->packet->writeInt32((*message)->getWireFormatSize(false));

    (*message)->packet = this->packet;
    (*message)->toWireFormat(false);
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
    size += sizeof(long int) + sizeof(int);
    size += (*message)->getWireFormatSize(false);
  }

  return size;
}

ostream& operator<< (ostream& os, const MessageSet& m)
{
  os << "MessageSet.numMessages:" << m.messages.size() << "\n";
  for(vector<Message*>::const_iterator message=m.messages.begin(); message!=m.messages.end(); ++message)
  {
    os << *(*message);
  }
  return os;
}

}; // namespace LibKafka
