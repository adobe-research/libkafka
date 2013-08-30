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

#include <iostream>

#include "FetchRequest.h"
#include "../ApiConstants.h"

using namespace std;

namespace LibKafka {

  FetchRequest::FetchRequest(unsigned char *buffer, bool releaseBuffer) : Request(buffer, releaseBuffer)
  {
    D(cout.flush() << "--------------FetchRequest(buffer)\n";)

    // Kafka Protocol: int replicaId
    this->replicaId = this->packet->readInt32();

    // Kafka Protocol: int maxWaitTime
    this->maxWaitTime = this->packet->readInt32();

    // Kafka Protocol: int minBytes
    this->minBytes = this->packet->readInt32();

    // Kafka Protocol: TopicNameBlock[] (note TopicNameBlock is not a protocol-defined structure)
    this->fetchTopicArraySize = this->packet->readInt32();
    this->fetchTopicArray = new TopicNameBlock<FetchPartition>*[this->fetchTopicArraySize];
    for (int i=0; i<this->fetchTopicArraySize; i++) {
      this->fetchTopicArray[i] = new TopicNameBlock<FetchPartition>(this->packet);
    }
    this->releaseArrays = true;
  }

  FetchRequest::FetchRequest(int correlationId, std::string clientId, int replicaId, int maxWaitTime, int minBytes, int fetchTopicArraySize, TopicNameBlock<FetchPartition>** fetchTopicArray, bool releaseArrays) : Request(ApiConstants::FETCH_REQUEST_KEY, ApiConstants::API_VERSION, correlationId, clientId)
  {
    D(cout.flush() << "--------------FetchRequest(params)\n";)

    this->replicaId = replicaId;
    this->maxWaitTime = maxWaitTime;
    this->minBytes = minBytes;
    this->fetchTopicArraySize = fetchTopicArraySize;
    this->fetchTopicArray = fetchTopicArray;
    this->releaseArrays = releaseArrays;
  }

  FetchRequest::~FetchRequest()
  {
    if (this->releaseArrays)
    {
      for (int i=0; i<this->fetchTopicArraySize; i++) {
	delete this->fetchTopicArray[i];
      }
      delete[] this->fetchTopicArray;
    }
  }

  unsigned char* FetchRequest::toWireFormat(bool updatePacketSize)
  {
    unsigned char* buffer = this->Request::toWireFormat(false);

    D(cout.flush() << "--------------FetchRequest::toWireFormat()\n";)
    
    // Kafka Protocol: short int replicaId
    this->packet->writeInt32(this->replicaId);

    // Kafka Protocol: int maxWaitTime
    this->packet->writeInt32(this->maxWaitTime);

    // Kafka Protocol: int minBytes
    this->packet->writeInt32(this->minBytes);

    // Kafka Protocol: TopicNameBlock[] (note TopicNameBlock is not a protocol-defined structure)
    this->packet->writeInt32(this->fetchTopicArraySize);
    for (int i=0; i<this->fetchTopicArraySize; i++) {
      this->fetchTopicArray[i]->packet = this->packet;
      this->fetchTopicArray[i]->toWireFormat(false);
    }

    if (updatePacketSize) this->packet->updatePacketSize();
    return buffer;
  }

  int FetchRequest::getWireFormatSize(bool includePacketSize)
  {
    D(cout.flush() << "--------------FetchRequest::getWireFormatSize()\n";)

    // Request.getWireFormatSize
    // replicaId + maxWaitTime + minBytes + produceTopicArraySize*sizeof(TopicNameBlock<FetchPartition>)

    int size = Request::getWireFormatSize(includePacketSize);
    size += sizeof(int) + sizeof(int) + sizeof(int);
    size += sizeof(int);
    for (int i=0; i<fetchTopicArraySize; i++) {
      size += fetchTopicArray[i]->getWireFormatSize(false);
    }
    return size;
  }

  ostream& operator<< (ostream& os, const FetchRequest& fr)
  {
    os << (const Request&)fr;
    os << "FetchRequest.replicaId:" << fr.replicaId << "\n";
    os << "FetchRequest.maxWaitTime:" << fr.maxWaitTime << "\n";
    os << "FetchRequest.minBytes:" << fr.minBytes << "\n";
    os << "FetchRequest.fetchTopicArraySize:" << fr.fetchTopicArraySize << "\n";
    for (int i=0; i<fr.fetchTopicArraySize; i++) {
      os << "FetchRequest.fetchTopicArray[" << i << "]:" << *(fr.fetchTopicArray[i]) << "\n";
    }
    return os;
  }

}; // namespace LibKafka
