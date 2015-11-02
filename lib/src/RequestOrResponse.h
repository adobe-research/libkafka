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

#ifndef REQUESTORRESPONSE_H
#define REQUESTORRESPONSE_H

#include <string>
#include "Debug.h"
#include "Packet.h"
#include "WireFormatter.h"
#include "PacketWriter.h"

namespace LibKafka {

class RequestOrResponse : public WireFormatter, public PacketWriter
{
  public:

    RequestOrResponse();
    RequestOrResponse(long bufferSize);
    RequestOrResponse(unsigned char *buffer, bool releaseBuffer = false);
    ~RequestOrResponse();

    int size(bool includeProtocolSizeFieldLength = true) const;

    unsigned char* toWireFormat(bool updatePacketSize = true);
    int getWireFormatSize(bool includePacketSize = true);
};

std::ostream& operator<< (std::ostream& os, const RequestOrResponse& r);

}; // namespace LibKafka

#endif
