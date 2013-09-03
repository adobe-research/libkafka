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

#ifndef PACKET_H
#define PACKET_H

#include <string>
#include "Debug.h"
#include "WireFormatter.h"

namespace LibKafka {

class Packet : public WireFormatter
{
  public:

    static const int DEFAULT_BUFFER_SIZE = 1024;

    Packet(int bufferSize = DEFAULT_BUFFER_SIZE);
    Packet(unsigned char *buffer, bool releaseBuffer = false);
    ~Packet();

    unsigned char *getBuffer() { return this->buffer; }

    signed char readInt8();
    short int readInt16();
    int readInt32();
    long int readInt64();
    std::string readString();
    unsigned char* readBytes(int numBytes);

    void writeInt8(signed char value);
    void writeInt16(short int value);
    void writeInt32(int value);
    void writeInt64(int long value);
    void writeString(std::string value);
    void writeBytes(unsigned char* bytes, int numBytes);

    // see CRC32 usage assumptions in Packet.cc
    void beginCRC32();
    int endCRC32(); // returns *signed int* CRC32 calculation

    void seek(int numBytes); // pass over numBytes bytes

    void writeToFile(std::string filepath);

    int getSize(bool includeProtocolSizeFieldLength = true);
    void updatePacketSize();
    void resetForReading();

    unsigned char* toWireFormat(bool updateSize = true);
    int getWireFormatSize(bool includeSize = false);

  protected:
    
    int size; // size of packet, inclusive of initial protocol size int32 value

    unsigned char *buffer;
    unsigned char *head;

    unsigned char *crcField;
    unsigned char *crcHead;

  private:

    bool releaseBuffer;
};

}; // namespace LibKafka

#endif /* PACKET_H */
