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

#include <MetadataResponse.h>
#include <Broker.h>

using namespace std;

MetadataResponse::MetadataResponse(unsigned char *buffer, bool releaseBuffer) : Response(buffer, releaseBuffer)
{
  D(cout.flush() << "--------------MetadataResponse(buffer)\n";)

  // Kafka Protocol: Broker[] broker
  this->brokerArraySize = this->packet->readInt32();
  this->brokerArray = new Broker*[this->brokerArraySize];
  for (int i=0; i<this->brokerArraySize; i++) {
    this->brokerArray[i] = new Broker(this->packet);
  }

  // Kafka Protocol: TopicMetadata[] topicMetadata
  this->topicMetadataArraySize = this->packet->readInt32();
  this->topicMetadataArray = new TopicMetadata*[this->topicMetadataArraySize];
  for (int i=0; i<this->topicMetadataArraySize; i++) {
    this->topicMetadataArray[i] = new TopicMetadata(this->packet);
  }

  this->releaseArrays = true;
}

MetadataResponse::MetadataResponse(int correlationId, int brokerArraySize, Broker **brokerArray, int topicMetadataArraySize, TopicMetadata **topicMetadataArray) : Response(correlationId)
{
  D(cout.flush() << "--------------MetadataResponse(params)\n";)

  // Kafka Protocol: Broker[] brokers
  this->brokerArraySize = brokerArraySize;
  this->brokerArray = brokerArray;
  this->topicMetadataArraySize = topicMetadataArraySize;
  this->topicMetadataArray = topicMetadataArray;
  this->releaseArrays = false;
}

MetadataResponse::~MetadataResponse()
{
  if (releaseArrays) {
    for (int i=0; i<this->brokerArraySize; i++) {
      delete this->brokerArray[i];
    }
    delete this->brokerArray;
    for (int i=0; i<this->topicMetadataArraySize; i++) {
      delete this->topicMetadataArray[i];
    }
    delete this->topicMetadataArray;
  }
}

unsigned char* MetadataResponse::toWireFormat(bool updatePacketSize)
{
  unsigned char* buffer = this->Response::toWireFormat(false);

  D(cout.flush() << "--------------MetadataResponse::toWireFormat()\n";)

  // Kafka Protocol: Broker[] brokers
  this->packet->writeInt32(this->brokerArraySize);
  for (int i=0; i<this->brokerArraySize; i++) {
    this->brokerArray[i]->packet = this->packet;
    this->brokerArray[i]->toWireFormat(false);
  }
  
  // Kafka Protocol: TopicMetadata[] topicMetadata
  this->packet->writeInt32(this->topicMetadataArraySize);
  for (int i=0; i<this->topicMetadataArraySize; i++) {
    this->topicMetadataArray[i]->packet = this->packet;
    this->topicMetadataArray[i]->toWireFormat(false);
  }

  if (updatePacketSize) this->packet->updatePacketSize();
  return buffer;
}

int MetadataResponse::getWireFormatSize(bool includePacketSize)
{
  D(cout.flush() << "--------------MetadataResponse::getWireFormatSize()\n";)

  // Response.getWireFormatSize
  // brokerArraySize + brokerArraySize*sizeof(Broker)
  // topicMetadataArraySize + topicMetadataArraySize*sizeof(TopicMetadata)

  int size = Response::getWireFormatSize(includePacketSize);
  size += sizeof(int);
  for (int i=0; i<brokerArraySize; i++) {
    size += brokerArray[i]->getWireFormatSize(false);
  }
  size += sizeof(int);
  for (int i=0; i<topicMetadataArraySize; i++) {
    size += topicMetadataArray[i]->getWireFormatSize(false);
  }
  return size;
}

ostream& operator<< (ostream& os, const MetadataResponse& mr)
{
  os << (const Response&)mr;
  os << "MetadataResponse.brokerArraySize:" << mr.brokerArraySize << "\n";
  for (int i=0; i<mr.brokerArraySize; i++) {
    os << "MetadataResponse.brokerArray[" << i << "]:" << *(mr.brokerArray[i]) << "\n";
  }
  os << "MetadataResponse.topicMetadataArraySize:" << mr.topicMetadataArraySize << "\n";
  for (int i=0; i<mr.topicMetadataArraySize; i++) {
    os << "MetadataResponse.topicMetadataArray[" << i << "]:" << *(mr.topicMetadataArray[i]) << "\n";
  }
  return os;
}
