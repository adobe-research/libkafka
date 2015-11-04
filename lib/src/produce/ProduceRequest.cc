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

#include <iostream>

#include "ProduceRequest.h"
#include "../ApiConstants.h"

using namespace std;

namespace LibKafka {

  ProduceRequest::ProduceRequest(unsigned char *buffer, bool releaseBuffer) : Request(buffer, releaseBuffer)
  {
    D(cout.flush() << "--------------ProduceRequest(buffer)\n";)

    // Kafka Protocol: short int requiredAcks
    this->requiredAcks = this->packet->readInt16();

    // Kafka Protocol: int timeout
    this->timeout = this->packet->readInt32();

    // Kafka Protocol: TopicNameBlock<ProduceMessageSet>[] (note TopicNameBlock is not a protocol-defined structure)
    this->produceTopicArraySize = this->packet->readInt32();
    this->produceTopicArray = new TopicNameBlock<ProduceMessageSet>*[this->produceTopicArraySize];
    for (int i=0; i<this->produceTopicArraySize; i++) {
      this->produceTopicArray[i] = new TopicNameBlock<ProduceMessageSet>(this->packet);
    }
    this->releaseArrays = true;
  }

  ProduceRequest::ProduceRequest(int correlationId, std::string clientId, short int requiredAcks, int timeout, int produceTopicArraySize, TopicNameBlock<ProduceMessageSet>** produceTopicArray, bool releaseArrays) : Request(ApiConstants::PRODUCE_REQUEST_KEY, ApiConstants::API_VERSION, correlationId, clientId)
  {
    D(cout.flush() << "--------------ProduceRequest(params)\n";)

    this->requiredAcks = requiredAcks;
    this->timeout = timeout;
    this->produceTopicArraySize = produceTopicArraySize;
    this->produceTopicArray = produceTopicArray;
    this->releaseArrays = releaseArrays;
  }
  
  ProduceRequest::ProduceRequest(int correlationId, std::string clientId, short int requiredAcks, int timeout, int produceTopicArraySize, TopicNameBlock<ProduceMessageSet>** produceTopicArray, long bufferSize, bool releaseArrays) : Request(ApiConstants::PRODUCE_REQUEST_KEY, ApiConstants::API_VERSION, correlationId, clientId, bufferSize)
  {
    D(cout.flush() << "--------------ProduceRequest(params)\n";)

    this->requiredAcks = requiredAcks;
    this->timeout = timeout;
    this->produceTopicArraySize = produceTopicArraySize;
    this->produceTopicArray = produceTopicArray;
    this->releaseArrays = releaseArrays;
  }


  ProduceRequest::~ProduceRequest()
  {
    if (this->releaseArrays)
    {
      for (int i=0; i<this->produceTopicArraySize; i++) {
	delete this->produceTopicArray[i];
      }
      delete[] this->produceTopicArray;
    }
  }

  unsigned char* ProduceRequest::toWireFormat(bool updatePacketSize)
  {
    unsigned char* buffer = this->Request::toWireFormat(false);

    D(cout.flush() << "--------------ProduceRequest::toWireFormat()\n";)
    
    // Kafka Protocol: short int requiredAcks
    this->packet->writeInt16(this->requiredAcks);

    // Kafka Protocol: int timeout
    this->packet->writeInt32(this->timeout);

    // Kafka Protocol: TopicNameBlock<ProduceMessageSet>[] (note TopicNameBlock is not a protocol-defined structure)
    this->packet->writeInt32(this->produceTopicArraySize);
    for (int i=0; i<this->produceTopicArraySize; i++) {
      this->produceTopicArray[i]->packet = this->packet;
      this->produceTopicArray[i]->toWireFormat(false);
    }

    if (updatePacketSize) this->packet->updatePacketSize();
    return buffer;
  }

  int ProduceRequest::getWireFormatSize(bool includePacketSize)
  {
    D(cout.flush() << "--------------ProduceRequest::getWireFormatSize()\n";)

    // Request.getWireFormatSize
    // requiredAcks + timeout + produceTopicArraySize*sizeof(TopicNameBlock<ProduceMessageSet>)

    int size = Request::getWireFormatSize(includePacketSize);
    size += sizeof(short int) + sizeof(int);
    size += sizeof(int);
    for (int i=0; i<produceTopicArraySize; i++) {
      size += produceTopicArray[i]->getWireFormatSize(false);
    }
    return size;
  }

  void ProduceRequest::setCompression(int codec)
  {
    if ((codec != ApiConstants::MESSAGE_COMPRESSION_NONE) && (codec != ApiConstants::MESSAGE_COMPRESSION_GZIP) && (codec != ApiConstants::MESSAGE_COMPRESSION_SNAPPY))
    {
      E("ProduceRequest::setCompression():error:invalid codec:" << codec << "\n");
      return;
    }
    
    D(cout.flush() << "--------------ProduceRequest:setCompression():" << codec << "\n";)
    
    for (int i=0; i<this->produceTopicArraySize; i++) produceTopicArray[i]->setCompression(codec);
  }

  ostream& operator<< (ostream& os, const ProduceRequest& pr)
  {
    os << (const Request&)pr;
    os << "ProduceRequest.requiredAcks:" << pr.requiredAcks << "\n";
    os << "ProduceRequest.timeout:" << pr.timeout << "\n";
    os << "ProduceRequest.produceTopicArraySize:" << pr.produceTopicArraySize << "\n";
    for (int i=0; i<pr.produceTopicArraySize; i++) {
      os << "ProduceRequest.produceTopicArray[" << i << "]:" << *(pr.produceTopicArray[i]) << "\n";
    }
    return os;
  }

}; // namespace LibKafka
