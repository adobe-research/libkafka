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
      D(cout.flush() << "--------------TopicNameBlock(buffer)\n";)

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
      D(cout.flush() << "--------------TopicNameBlock(params)\n";)

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
	D(cout.flush() << "--------------TopicNameBlock::toWireFormat()\n";)

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
	D(cout.flush() << "--------------TopicNameBlock::getWireFormatSize()\n";)

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
    os << tnb.topicName << ":" << tnb.subBlockArraySize;
    return os;
  }
  
  template<class SubBlockArrayClass>
  inline bool operator==(const TopicNameBlock<SubBlockArrayClass>& lhs, const TopicNameBlock<SubBlockArrayClass>& rhs) { return ((lhs.topicName==rhs.topicName)&&(lhs.subBlockArraySize==rhs.subBlockArraySize)); }
  
  template<class SubBlockArrayClass>
  inline bool operator!=(const TopicNameBlock<SubBlockArrayClass>& lhs, const TopicNameBlock<SubBlockArrayClass>& rhs) { return !operator==(lhs,rhs); }

}; // namespace LibKafka

#endif /* TOPICNAMEBLOCK_H */
