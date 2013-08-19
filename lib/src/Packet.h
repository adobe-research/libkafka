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

#ifndef PACKET_H
#define PACKET_H

#include <string>
#include <Debug.h>
#include <WireFormatter.h>

namespace LibKafka {

class Packet : public WireFormatter
{
  public:

    static const int DEFAULT_BUFFER_SIZE = 1024;


    Packet(int bufferSize = DEFAULT_BUFFER_SIZE);
    Packet(unsigned char *buffer, bool releaseBuffer = false);
    ~Packet();

    unsigned char *getBuffer() { return this->buffer; }

    short int readInt16();
    int readInt32();
    long int readInt64();
    std::string readString();

    void writeInt16(short int value);
    void writeInt32(int value);
    void writeInt64(int long value);
    void writeString(std::string value);
   
    int getSize(bool includeProtocolSizeFieldLength = true);
    void updatePacketSize();
    void resetForReading();

    unsigned char* toWireFormat(bool updateSize = true);
    int getWireFormatSize(bool includeSize = false);

  protected:
    
    int size; // size of packet, inclusive of initial protocol size int32 value

    unsigned char *buffer;
    unsigned char *head;

  private:

    bool releaseBuffer;
};

}; // namespace LibKafka

#endif /* PACKET_H */
