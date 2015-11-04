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

#ifndef PRODUCEREQUEST_H
#define PRODUCEREQUEST_H

#include <string>
#include "../Request.h"
#include "../TopicNameBlock.h"
#include "../ApiConstants.h"
#include "ProduceMessageSet.h"

namespace LibKafka {

class ProduceRequest : public Request
{
  public:

    short int requiredAcks;
    int timeout;

    int produceTopicArraySize;
    TopicNameBlock<ProduceMessageSet> **produceTopicArray;

    ProduceRequest(unsigned char *buffer, bool releaseBuffer = false);
    ProduceRequest(int correlationId, std::string clientId, short int requiredAcks, int timeout, int produceTopicArraySize, TopicNameBlock<ProduceMessageSet> **produceTopicArray, bool releaseArrays = false);
    ProduceRequest(int correlationId, std::string clientId, short int requiredAcks, int timeout, int produceTopicArraySize, TopicNameBlock<ProduceMessageSet> **produceTopicArray, long bufferSize, bool releaseArrays = false);
    ~ProduceRequest();

    unsigned char* toWireFormat(bool updatePacketSize = true);
    int getWireFormatSize(bool includePacketSize = true);
    
    void setCompression(int codec); // either MESSAGE_COMPRESSION_NONE, MESSAGE_COMPRESSION_GZIP, or MESSAGE_COMPRESSION_SNAPPY

  private:

    bool releaseArrays;
};

std::ostream& operator<< (std::ostream& os, const ProduceRequest& pr);

}; // namespace LibKafka

#endif
