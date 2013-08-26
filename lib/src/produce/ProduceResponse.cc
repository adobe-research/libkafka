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

#include <ProduceResponse.h>
#include <ProduceResponseTopic.h>

using namespace std;

namespace LibKafka {

ProduceResponse::ProduceResponse(unsigned char *buffer, bool releaseBuffer) : Response(buffer, releaseBuffer)
{
  D(cout.flush() << "--------------ProduceResponse(buffer)\n";)

  // Kafka Protocol: ProduceResponseTopic[] produceResponseTopicArray
  this->produceResponseTopicArraySize = this->packet->readInt32();
  this->produceResponseTopicArray = new ProduceResponseTopic*[this->produceResponseTopicArraySize];
  for (int i=0; i<this->produceResponseTopicArraySize; i++) {
    this->produceResponseTopicArray[i] = new ProduceResponseTopic(this->packet);
  }

  this->releaseArrays = true;
}

ProduceResponse::ProduceResponse(int correlationId, int produceResponseTopicArraySize, ProduceResponseTopic **produceResponseTopicArray, bool releaseArrays) : Response(correlationId)
{
  D(cout.flush() << "--------------ProduceResponse(params)\n";)

  // Kafka Protocol: ProduceResponseTopic[] produceResponseTopicArray
  this->produceResponseTopicArraySize = produceResponseTopicArraySize;
  this->produceResponseTopicArray = produceResponseTopicArray;
  this->releaseArrays = releaseArrays;
}

ProduceResponse::~ProduceResponse()
{
  if (releaseArrays) {
    for (int i=0; i<this->produceResponseTopicArraySize; i++) {
      delete this->produceResponseTopicArray[i];
    }
    delete[] this->produceResponseTopicArray;
  }
}

unsigned char* ProduceResponse::toWireFormat(bool updatePacketSize)
{
  unsigned char* buffer = this->Response::toWireFormat(false);

  D(cout.flush() << "--------------ProduceResponse::toWireFormat()\n";)

  // Kafka Protocol: ProduceResponseTopic[] produceResponseTopicArray
  this->packet->writeInt32(this->produceResponseTopicArraySize);
  for (int i=0; i<this->produceResponseTopicArraySize; i++) {
    this->produceResponseTopicArray[i]->packet = this->packet;
    this->produceResponseTopicArray[i]->toWireFormat(false);
  }

  if (updatePacketSize) this->packet->updatePacketSize();
  return buffer;
}

int ProduceResponse::getWireFormatSize(bool includePacketSize)
{
  D(cout.flush() << "--------------ProduceResponse::getWireFormatSize()\n";)

  // Response.getWireFormatSize
  // produceResponseTopicArraySize + produceResponseTopicArraySize*sizeof(ProduceResponseTopic)

  int size = Response::getWireFormatSize(includePacketSize);
  size += sizeof(int);
  for (int i=0; i<produceResponseTopicArraySize; i++) {
    size += produceResponseTopicArray[i]->getWireFormatSize(false);
  }
  return size;
}

ostream& operator<< (ostream& os, const ProduceResponse& pr)
{
  os << (const Response&)pr;
  os << "ProduceResponse.produceResponseTopicArraySize:" << pr.produceResponseTopicArraySize << "\n";
  for (int i=0; i<pr.produceResponseTopicArraySize; i++) {
    os << "ProduceResponse.produceResponseTopicArray[" << i << "]:" << *(pr.produceResponseTopicArray[i]) << "\n";
  }
  return os;
}

}; // namespace LibKafka
