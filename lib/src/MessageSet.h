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

#ifndef MESSAGESET_H
#define MESSAGESET_H

#include <string>

#include "Debug.h"
#include "Packet.h"
#include "WireFormatter.h"
#include "PacketWriter.h"

namespace LibKafka {

class MessageSet: public WireFormatter, public PacketWriter
{
  public:

    long int offset;
    int messageSize; // exclusive of offset and messageSize protocol lengths
    int crc;
    unsigned char magicByte;
    unsigned char attributes;
    int keyLength;
    unsigned char* key;
    int valueLength;
    unsigned char* value;

    MessageSet(Packet *packet);
    MessageSet(long int offset, int messageSize, int crc, unsigned char magicByte, unsigned char attributes, int keyLength, unsigned char* key, int valueLength, unsigned char* value, bool releaseArrays = false);
    ~MessageSet();

    unsigned char* toWireFormat(bool updatePacketSize = true);
    int getWireFormatSize(bool includePacketSize = false);

  private:

    bool releaseArrays;
};

std::ostream& operator<< (std::ostream& os, const MessageSet& t);
inline bool operator==(const MessageSet& lhs, const MessageSet& rhs) { return ((lhs.offset==rhs.offset)&&(lhs.messageSize==rhs.messageSize)&&(lhs.crc==rhs.crc)&&(lhs.magicByte==rhs.magicByte)&&(lhs.attributes==rhs.attributes)); }
inline bool operator!=(const MessageSet& lhs, const MessageSet& rhs) { return !operator==(lhs,rhs); }

}; // namespace LibKafka

#endif /* MESSAGESET_H */
