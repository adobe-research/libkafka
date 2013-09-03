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

#ifndef TOPICNAMEBLOCK_H
#define TOPICNAMEBLOCK_H

#include <string>
#include "Debug.h"
#include "Packet.h"
#include "WireFormatter.h"
#include "PacketWriter.h"

namespace LibKafka {

  template<class SubBlockArrayClass>
  class TopicNameBlock : public WireFormatter, public PacketWriter
  {
    public:

      std::string topicName;
      int subBlockArraySize;
      SubBlockArrayClass **subBlockArray;

      TopicNameBlock(Packet *packet) : WireFormatter(), PacketWriter(packet)
    {
      D(std::cout.flush() << "--------------TopicNameBlock(buffer)\n";)

	// Kafka Protocol: kafka string topicName
	this->topicName = this->packet->readString();

      // Kafka Protocol: SubBlockArrayClass[]
      this->subBlockArraySize = this->packet->readInt32();
      this->subBlockArray = new SubBlockArrayClass*[this->subBlockArraySize];
      for (int i=0; i<this->subBlockArraySize; i++) {
	this->subBlockArray[i] = new SubBlockArrayClass(this->packet);
      }

      this->releaseArrays = true;
    }

      TopicNameBlock(std::string topicName, int subBlockArraySize, SubBlockArrayClass **subBlockArray, bool releaseArrays = false) : WireFormatter(), PacketWriter()
    {
      D(std::cout.flush() << "--------------TopicNameBlock(params)\n";)

	this->topicName = topicName;
      this->subBlockArraySize = subBlockArraySize;
      this->subBlockArray = subBlockArray;
      this->releaseArrays = releaseArrays;
    }

      ~TopicNameBlock()
      {
	if (this->releaseArrays)
	{
	  for (int i=0; i<this->subBlockArraySize; i++) {
	    delete this->subBlockArray[i];
	  }
	  delete[] this->subBlockArray;
	}
      }

      unsigned char* toWireFormat(bool updatePacketSize = true)
      {
	D(std::cout.flush() << "--------------TopicNameBlock::toWireFormat()\n";)

	  // Kafka Protocol: kafka string topicName
	  this->packet->writeString(this->topicName);

	// Kafka Protocol: SubBlockArrayClass[]
	this->packet->writeInt32(this->subBlockArraySize);
	for (int i=0; i<this->subBlockArraySize; i++) {
	  this->subBlockArray[i]->packet = this->packet;
	  this->subBlockArray[i]->toWireFormat(false);
	}

	if (updatePacketSize) this->packet->updatePacketSize();
	return this->packet->getBuffer();
      }

      int getWireFormatSize(bool includePacketSize = false)
      {
	D(std::cout.flush() << "--------------TopicNameBlock::getWireFormatSize()\n";)

	  // Packet.size
	  // topicName
	  // subBlockArraySize*SubBlockArrayClass

	  int size = 0;
	if (includePacketSize) size += sizeof(int);
	size += sizeof(short int) + this->topicName.length() + sizeof(int);
	for (int i=0; i<subBlockArraySize; i++) {
	  size += subBlockArray[i]->getWireFormatSize(false);
	}
	return size;
      }

    private:

      bool releaseArrays;
  };

  template<class SubBlockArrayClass>
  std::ostream& operator<< (std::ostream& os, const TopicNameBlock<SubBlockArrayClass>& tnb)
  {
    os << "TopicNameBlock.topicName:" << tnb.topicName << "\n";
    os << "TopicNameBlock.subBlockArraySize:" << tnb.subBlockArraySize << "\n";
    for (int i=0; i<tnb.subBlockArraySize; i++) {
      os << "TopicNameBlock.subBlockArray[" << i << "]:" << *(tnb.subBlockArray[i]);
    }
    return os;
  }
  
  template<class SubBlockArrayClass>
  inline bool operator==(const TopicNameBlock<SubBlockArrayClass>& lhs, const TopicNameBlock<SubBlockArrayClass>& rhs) { return ((lhs.topicName==rhs.topicName)&&(lhs.subBlockArraySize==rhs.subBlockArraySize)); }
  
  template<class SubBlockArrayClass>
  inline bool operator!=(const TopicNameBlock<SubBlockArrayClass>& lhs, const TopicNameBlock<SubBlockArrayClass>& rhs) { return !operator==(lhs,rhs); }

}; // namespace LibKafka

#endif /* TOPICNAMEBLOCK_H */
