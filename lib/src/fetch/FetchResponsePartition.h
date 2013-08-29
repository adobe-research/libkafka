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

#ifndef FETCHRESPONSEPARTITION_H
#define FETCHRESPONSEPARTITION_H

#include <string>
#include "../Debug.h"
#include "../Packet.h"
#include "../MessageSet.h"
#include "../WireFormatter.h"
#include "../PacketWriter.h"

namespace LibKafka {

class FetchResponsePartition : public WireFormatter, public PacketWriter
{
  public:

    int partition;
    short int errorCode;
    long int highwaterMarkOffset;
    int messageSetSize;
    MessageSet *messageSet;

    FetchResponsePartition(Packet *packet);
    FetchResponsePartition(int partition, short int errorCode, long int highwaterMarkOffset, int messageSetSize, MessageSet *messageSet, bool releaseArrays = false);
    ~FetchResponsePartition();

    unsigned char* toWireFormat(bool updatePacketSize = true);
    int getWireFormatSize(bool includePacketSize = false);

  private:

    bool releaseArrays;
};

std::ostream& operator<< (std::ostream& os, const FetchResponsePartition& frm);
inline bool operator==(const FetchResponsePartition& lhs, const FetchResponsePartition& rhs) { return ((lhs.partition==rhs.partition)&&(lhs.errorCode==rhs.errorCode)&&(lhs.highwaterMarkOffset==rhs.highwaterMarkOffset)&&(lhs.highwaterMarkOffset==rhs.highwaterMarkOffset)&&(lhs.messageSetSize==rhs.messageSetSize)); }
inline bool operator!=(const FetchResponsePartition& lhs, const FetchResponsePartition& rhs) { return !operator==(lhs,rhs); }

}; // namespace LibKafka

#endif /* FETCHRESPONSEPARTITION_H */
