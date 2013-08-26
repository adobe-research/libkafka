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

#ifndef TOPICMETADATA_H
#define TOPICMETADATA_H

#include <string>
#include <Debug.h>
#include <Packet.h>
#include <WireFormatter.h>
#include <PacketWriter.h>
#include <PartitionMetadata.h>

namespace LibKafka {

class TopicMetadata : public WireFormatter, public PacketWriter
{
  public:

    short int topicErrorCode;
    std::string topicName;
    int partitionMetadataArraySize;
    PartitionMetadata **partitionMetadataArray;

    TopicMetadata(Packet *packet);
    TopicMetadata(short int topicErrorCode, std::string topicName, int partitionMetadataArraySize, PartitionMetadata **partitionMetadataArray, bool releaseArrays = false);
    ~TopicMetadata();

    unsigned char* toWireFormat(bool updatePacketSize = true);
    int getWireFormatSize(bool includePacketSize = false);

  private:

    bool releaseArrays;
};

std::ostream& operator<< (std::ostream& os, const TopicMetadata& b);
inline bool operator==(const TopicMetadata& lhs, const TopicMetadata& rhs) { return ((lhs.topicErrorCode==rhs.topicErrorCode)&&(lhs.topicName==rhs.topicName)); }
inline bool operator!=(const TopicMetadata& lhs, const TopicMetadata& rhs) { return !operator==(lhs,rhs); }

}; // namespace LibKafka

#endif /* TOPICMETADATA_H */
