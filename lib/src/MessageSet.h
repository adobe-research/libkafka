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

#ifndef MESSAGESET_H
#define MESSAGESET_H

#include <string>
#include <vector>

#include "Debug.h"
#include "Packet.h"
#include "WireFormatter.h"
#include "PacketWriter.h"
#include "Message.h"

namespace LibKafka {

class MessageSet: public WireFormatter, public PacketWriter
{
  public:

    int messageSetSize;
    std::vector<Message*> messages;

    MessageSet(int messageSetSize, Packet *packet);
    MessageSet(int messageSetSize, std::vector<Message*> messages, bool releaseArrays = false);
    ~MessageSet();

    unsigned char* toWireFormat(bool updatePacketSize = true);
    int getWireFormatSize(bool includePacketSize = false);

    void setCompression(int codec);

  private:

    bool releaseArrays;
};

std::ostream& operator<< (std::ostream& os, const MessageSet& t);
inline bool operator==(const MessageSet& lhs, const MessageSet& rhs) { return ((lhs.messageSetSize==rhs.messageSetSize)&&(lhs.messages.size()==rhs.messages.size())); }
inline bool operator!=(const MessageSet& lhs, const MessageSet& rhs) { return !operator==(lhs,rhs); }

}; // namespace LibKafka

#endif /* MESSAGESET_H */
