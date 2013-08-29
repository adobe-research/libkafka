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
#include <vector>

#include <FetchResponse.h>

using namespace std;

namespace LibKafka {

FetchResponse::FetchResponse(unsigned char *buffer, bool releaseBuffer) : Response(buffer, releaseBuffer)
{
  D(cout.flush() << "--------------FetchResponse(buffer)\n";)

  // Kafka Protocol: TopicNameBlock<FetchResponsePartition>[] fetchResponseTopicArray
  this->fetchResponseTopicArraySize = this->packet->readInt32();
  this->fetchResponseTopicArray = new TopicNameBlock<FetchResponsePartition>*[this->fetchResponseTopicArraySize];
  for (int i=0; i<this->fetchResponseTopicArraySize; i++) {
    this->fetchResponseTopicArray[i] = new TopicNameBlock<FetchResponsePartition>(this->packet);
  }

  this->releaseArrays = true;
}

FetchResponse::FetchResponse(int correlationId, int fetchResponseTopicArraySize, TopicNameBlock<FetchResponsePartition> **fetchResponseTopicArray, bool releaseArrays) : Response(correlationId)
{
  D(cout.flush() << "--------------FetchResponse(params)\n";)

  // Kafka Protocol: TopicNameBlock<FetchResponsePartition>[] fetchResponseTopicArray
  this->fetchResponseTopicArraySize = fetchResponseTopicArraySize;
  this->fetchResponseTopicArray = fetchResponseTopicArray;
  this->releaseArrays = releaseArrays;
}

FetchResponse::~FetchResponse()
{
  if (releaseArrays) {
    for (int i=0; i<this->fetchResponseTopicArraySize; i++) {
      delete this->fetchResponseTopicArray[i];
    }
    delete[] this->fetchResponseTopicArray;
  }
}

unsigned char* FetchResponse::toWireFormat(bool updatePacketSize)
{
  unsigned char* buffer = this->Response::toWireFormat(false);

  D(cout.flush() << "--------------FetchResponse::toWireFormat()\n";)

  // Kafka Protocol: TopicNameBlock<FetchResponsePartition>[] fetchResponseTopicArray
  this->packet->writeInt32(this->fetchResponseTopicArraySize);
  for (int i=0; i<this->fetchResponseTopicArraySize; i++) {
    this->fetchResponseTopicArray[i]->packet = this->packet;
    this->fetchResponseTopicArray[i]->toWireFormat(false);
  }

  if (updatePacketSize) this->packet->updatePacketSize();
  return buffer;
}

int FetchResponse::getWireFormatSize(bool includePacketSize)
{
  D(cout.flush() << "--------------FetchResponse::getWireFormatSize()\n";)

  // Response.getWireFormatSize
  // fetchResponseTopicArraySize + fetchResponseTopicArraySize*sizeof(TopicNameBlock<FetchResponsePartition>)

  int size = Response::getWireFormatSize(includePacketSize);
  size += sizeof(int);
  for (int i=0; i<fetchResponseTopicArraySize; i++) {
    size += fetchResponseTopicArray[i]->getWireFormatSize(false);
  }
  return size;
}

ostream& operator<< (ostream& os, const FetchResponse& pr)
{
  os << (const Response&)pr;
  os << "FetchResponse.fetchResponseTopicArraySize:" << pr.fetchResponseTopicArraySize << "\n";
  for (int i=0; i<pr.fetchResponseTopicArraySize; i++) {
    os << "FetchResponse.fetchResponseTopicArray[" << i << "]:" << *(pr.fetchResponseTopicArray[i]) << "\n";
  }
  return os;
}

}; // namespace LibKafka
