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

#ifndef MESSAGE_H
#define MESSAGE_H

#include <string>

#include "Debug.h"
#include "Packet.h"
#include "WireFormatter.h"
#include "PacketWriter.h"

namespace LibKafka {

class Message: public WireFormatter, public PacketWriter
{
  public:

    const static unsigned char COMPRESSION_MASK = 0x7;

    int crc;
    unsigned char magicByte;
    unsigned char attributes;
    int keyLength;
    unsigned char* key;
    int valueLength;
    unsigned char* value;

    long int offset;

    Message(Packet *packet, long int offset = -1);
    Message(int crc, unsigned char magicByte, unsigned char attributes, int keyLength, unsigned char* key, int valueLength, unsigned char* value, long int offset = -1, bool releaseArrays = false);
    ~Message();

    unsigned char* toWireFormat(bool updatePacketSize = true);
    int getWireFormatSize(bool includePacketSize = false);

  private:

    bool releaseArrays;
};

std::ostream& operator<< (std::ostream& os, const Message& t);
inline bool operator==(const Message& lhs, const Message& rhs) { return ((lhs.crc==rhs.crc)&&(lhs.magicByte==rhs.magicByte)&&(lhs.attributes==rhs.attributes)&&(lhs.keyLength==rhs.keyLength)&&(lhs.valueLength==rhs.valueLength)); }
inline bool operator!=(const Message& lhs, const Message& rhs) { return !operator==(lhs,rhs); }

}; // namespace LibKafka

#endif /* MESSAGE_H */
