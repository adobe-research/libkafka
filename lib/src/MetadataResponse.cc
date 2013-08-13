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

MetadataResponse::MetadataResponse(unsigned char *buffer) : Response(buffer)
{
  D(cout << "--------------MetadataResponse(buffer)\n";)

  // Kafka Protocol: Broker[] brokers
  this->brokerArraySize = this->packet->readInt32();
  this->brokerArray = new Broker*[this->brokerArraySize];
  for (int i=0; i<this->brokerArraySize; i++) {
    this->brokerArray[i] = new Broker(this->packet);
  }
  this->releaseArrays = true;
}

MetadataResponse::MetadataResponse(int correlationId, int brokerArraySize, Broker **brokerArray, int topicMetadataArraySize, TopicMetadata **topicMetadataArray) : Response(correlationId)
{
  D(cout << "--------------MetadataResponse(params)\n";)

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
  }
}

unsigned char* MetadataResponse::toWireFormat(bool updateSize)
{
  unsigned char* buffer = this->Response::toWireFormat(false);

  D(cout << "--------------MetadataResponse::toWireFormat()\n";)

    // Kafka Protocol: Broker[] brokers
    this->packet->writeInt32(this->brokerArraySize);
  for (int i=0; i<this->brokerArraySize; i++) {
    this->brokerArray[i]->packet = this->packet;
    this->brokerArray[i]->toWireFormat(false);
  }

  if (updateSize) this->packet->updatePacketSize();
  return buffer;
}
