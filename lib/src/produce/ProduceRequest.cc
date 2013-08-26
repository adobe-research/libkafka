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

#include <ProduceRequest.h>
#include <ApiConstants.h>

using namespace std;

namespace LibKafka {

  ProduceRequest::ProduceRequest(unsigned char *buffer, bool releaseBuffer) : Request(buffer, releaseBuffer)
  {
    D(cout.flush() << "--------------ProduceRequest(buffer)\n";)

    // Kafka Protocol: short int requiredAcks
    this->requiredAcks = this->packet->readInt16();

    // Kafka Protocol: int timeout
    this->timeout = this->packet->readInt32();

    // Kafka Protocol: ProduceTopic[] (note ProduceTopic is not a protocol-defined structure)
    this->produceTopicArraySize = this->packet->readInt32();
    this->produceTopicArray = new ProduceTopic*[this->produceTopicArraySize];
    for (int i=0; i<this->produceTopicArraySize; i++) {
      this->produceTopicArray[i] = new ProduceTopic(this->packet);
    }
    this->releaseArrays = true;
  }

  ProduceRequest::ProduceRequest(int correlationId, std::string clientId, short int requiredAcks, int timeout, int produceTopicArraySize, ProduceTopic** produceTopicArray, bool releaseArrays) : Request(ApiConstants::PRODUCE_REQUEST_KEY, ApiConstants::API_VERSION, correlationId, clientId)
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

    // Kafka Protocol: ProduceTopic[] (note ProduceTopic is not a protocol-defined structure)
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
    // requiredAcks + timeout + produceTopicArraySize*sizeof(ProduceTopic)

    int size = Request::getWireFormatSize(includePacketSize);
    size += sizeof(short int) + sizeof(int);
    size += sizeof(int);
    for (int i=0; i<produceTopicArraySize; i++) {
      size += produceTopicArray[i]->getWireFormatSize(false);
    }
    return size;
  }

  ostream& operator<< (ostream& os, const ProduceRequest& pr)
  {
    os << (const Request&)pr;
    os << "ProduceRequest.requiredAcks:" << pr.requiredAcks << "\n";
    os << "ProduceRequest.timeout:" << pr.timeout << "\n";
    os << "ProduceRequest.produceTopicArraySize:" << pr.produceTopicArraySize << "\n";
    for (int i=0; i<pr.produceTopicArraySize; i++) {
      os << "ProduceRequest.produceTopicArray[" << i << "]:" << pr.produceTopicArray[i] << "\n";
    }
    return os;
  }

}; // namespace LibKafka
