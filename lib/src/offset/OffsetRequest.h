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

#ifndef OFFSETREQUEST_H
#define OFFSETREQUEST_H

#include <string>
#include "../Request.h"
#include "../TopicNameBlock.h"
#include "OffsetPartition.h"

namespace LibKafka {

class OffsetRequest : public Request
{
  public:

    int replicaId;

    int offsetTopicArraySize;
    TopicNameBlock<OffsetPartition> **offsetTopicArray;

    OffsetRequest(unsigned char *buffer, bool releaseBuffer = false);
    OffsetRequest(int correlationId, std::string clientId, int replicaId, int offsetTopicArraySize, TopicNameBlock<OffsetPartition> **offsetTopicArray, bool releaseArrays = false);
    ~OffsetRequest();

    unsigned char* toWireFormat(bool updatePacketSize = true);
    int getWireFormatSize(bool includePacketSize = true);

  private:

    bool releaseArrays;
};

std::ostream& operator<< (std::ostream& os, const OffsetRequest& ofr);

}; // namespace LibKafka

#endif
