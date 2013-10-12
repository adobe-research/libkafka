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

#include <string>
#include <cstring>
#include <iostream>
#include <fstream>
#include <arpa/inet.h>
#include <zlib.h>
#include <snappy.h>

#include "Packet.h"
#include "Util.h"

using namespace std;

namespace LibKafka {

const int Packet::DEFAULT_BUFFER_SIZE;

// Constructor to parse incoming Kafka protocol packet
Packet::Packet(unsigned char *buffer, bool releaseBuffer) : WireFormatter()
{
  D(cout.flush() << "--------------Packet(incoming)\n";)

  this->buffer = buffer;
  this->head = buffer;
  this->size = readInt32() + sizeof(int); // protocol size field is exclusive of size field length, size instance variable is inclusive of size field length
  this->releaseBuffer = releaseBuffer;
}

// Constructor to construct outgoing Kafka protocol packet
Packet::Packet(int bufferSize) : WireFormatter()
{
  D(cout.flush() << "--------------Packet(outgoing)\n";)

  buffer = new unsigned char[bufferSize];
  head = buffer;
  this->size = 0;
  writeInt32(this->size);

  this->releaseBuffer = true;
}

int Packet::getWireFormatSize(bool includeSize)
{
  D(cout.flush() << "--------------Packet::getWireFormatSize()\n";)

  // Packet.size

  int size = 0;
  if (includeSize) size += sizeof(int);
  return size;
}

Packet::~Packet()
{
  D(cout.flush() << "--------------~Packet()\n";)

  if (releaseBuffer) delete[] buffer;
}

unsigned char* Packet::toWireFormat(bool updateSize)
{
  D(cout.flush() << "--------------Packet::toWireFormat()\n";)

  if (updateSize) updatePacketSize();
  return this->buffer;
}

signed char Packet::readInt8()
{
  signed char netValue = *(signed char*)(this->head);
  // netValue == hostValue for single bytes
  this->head += sizeof(signed char);
  D(cout.flush() << "Packet::readInt8():netValue(" << netValue << "):hostValue(" << netValue << ")\n";)
  return netValue;
}

short int Packet::readInt16()
{
  short int netValue = *(int*)(this->head);
  short int hostValue = ntohs(netValue);
  this->head += sizeof(short int);
  D(cout.flush() << "Packet::readInt16():netValue(" << netValue << "):hostValue(" << hostValue << ")\n";)
  return hostValue;
}

int Packet::readInt32()
{
  int netValue = *(int*)(this->head);
  int hostValue = ntohl(netValue);
  this->head += sizeof(int);
  D(cout.flush() << "Packet::readInt32():netValue(" << netValue << "):hostValue(" << hostValue << ")\n";)
  return hostValue;
}

long int Packet::readInt64()
{
  long int netValue = *(long int*)(this->head);
  long int hostValue = ntohll(netValue);
  this->head += sizeof(long int);
  D(cout.flush() << "Packet::readInt64():netValue(" << netValue << "):hostValue(" << hostValue << ")\n";)
  return hostValue;
}

string Packet::readString()
{
  short int length = readInt16();
  string value = string((const char *)(this->head), length);
  this->head += length;
  D(cout.flush() << "Packet::readString():" << length << ":" << value << "\n";)
  return value;
}

unsigned char* Packet::readBytes(int numBytes)
{
  // returns a pointer to the bytes within the Packet buffer, and increments head
  if (numBytes < 0) return this->head; // fix for issue 1, reported by zicfyy@gmail.com
  unsigned char* bytes = this->head;
  this->head += numBytes;
  D(cout.flush() << "Packet::readBytes():" << numBytes << "\n";)
  return bytes;
}

void Packet::updatePacketSize()
{
  int netValue = htonl((this->size) - sizeof(int)); // prior to sending, set packetSize exclusive of size (int)
  memcpy(buffer, &netValue, sizeof(int));
  D(cout.flush() << "Packet::updatePacketSize():hostValue(" << this->size << "):netValue(" << netValue << ")\n";)
}

void Packet::writeInt8(signed char hostValue)
{
  // netValue == hostValue for single bytes
  memcpy(head, &hostValue, sizeof(signed char));
  head += sizeof(signed char);
  this->size += sizeof(signed char);
  D(cout.flush() << "Packet::writeInt8():hostValue(" << hostValue << "):netValue(" << hostValue << ")\n";)
}

void Packet::writeInt16(short int hostValue)
{
  short int netValue = htons(hostValue);
  memcpy(head, &netValue, sizeof(short int));
  head += sizeof(short int);
  this->size += sizeof(short int);
  D(cout.flush() << "Packet::writeInt16():hostValue(" << hostValue << "):netValue(" << netValue << ")\n";)
}

void Packet::writeInt32(int hostValue)
{
  int netValue = htonl(hostValue);
  memcpy(head, &netValue, sizeof(int));
  head += sizeof(int);
  this->size += sizeof(int);
  D(cout.flush() << "Packet::writeInt32():hostValue(" << hostValue << "):netValue(" << netValue << ")\n";)
}

void Packet::updateInt32(int hostValue, unsigned char *bufferPointer)
{
  // Assumption: for retroactive updates to previously wtitten fields within the Packet, using value from getHead() prior to initial write
  int netValue = htonl(hostValue);
  memcpy(bufferPointer, &netValue, sizeof(int));
  D(cout.flush() << "Packet::updateInt32():hostValue(" << hostValue << "):netValue(" << netValue << ")\n";)
}

void Packet::writeInt64(long int hostValue)
{
  long int netValue = htonll(hostValue);
  memcpy(head, &netValue, sizeof(long int));
  head += sizeof(long int);
  this->size += sizeof(long int);
  D(cout.flush() << "Packet::writeInt64():Value(" << hostValue << "):netValue(" << netValue << ")\n";)
}

void Packet::writeString(string value)
{
  short int length = value.length();
  writeInt16(length);
  memcpy(head, value.c_str(), length);
  head += length;
  this->size += length;
  D(cout.flush() << "Packet::writeString():" << length << ":" << value.c_str() << "\n";)
}

// The writeBytes() function makes the following assumption:
// 1 - the length of the byte array will be written as an int32 prior to the byte array, as per the spec.
// 2 - other protocol array types must explicitly write their length as an int32 prior to writing their array payload
void Packet::writeBytes(unsigned char* bytes, int numBytes)
{
  this->writeInt32(numBytes);
  memcpy(head, bytes, numBytes);
  head += numBytes;
  this->size += numBytes;
  D(cout.flush() << "Packet::writeBytes():" << numBytes << "\n";)
}

int Packet::writeCompressedBytes(unsigned char* bytes, int numBytes, CompressionType codec)
{
  if (codec == COMPRESSION_GZIP)
  {
    // z_stream initialization as per zlib.h comments
    z_stream zInfo = {0};
    zInfo.zalloc = Z_NULL;
    zInfo.zfree = Z_NULL;
    zInfo.opaque = Z_NULL;
    zInfo.total_out = 0;
    zInfo.next_in = bytes;
    zInfo.avail_in = numBytes;

    // use deflateInit2() to generate GZIP headers around compressed content
    int status = deflateInit2(&zInfo, Z_DEFAULT_COMPRESSION, Z_DEFLATED, (15+16), 8, Z_DEFAULT_STRATEGY);
    if (status != Z_OK)
    {
      E("Packet::writeCompressedBytes():error:GZIP deflateInit2() failure:errno: " << status << "\n");
      return -1;
    }
   
    // compression buffer size allocation as per zlib.h sizing comments
    unsigned long compressionBufferSize = MAX((int)((float)numBytes * 1.01) + 12, 512);
    D(cout.flush() << "Packet::writeCompressedBytes():GZIP:compressionBufferSize:" << compressionBufferSize << "\n";)
    unsigned char* compressionBuffer = new unsigned char[compressionBufferSize];

    // deflate may return partial results
    while (status == Z_OK)
    {
      zInfo.next_out = compressionBuffer + zInfo.total_out;
      zInfo.avail_out = compressionBufferSize - zInfo.total_out;
      status = deflate(&zInfo, Z_FINISH);
    }

    if (status != Z_STREAM_END)
    {
      E("Packet::writeCompressedBytes():error:GZIP compression error, status = " << status << "\n");
      delete[] compressionBuffer;
      deflateEnd(&zInfo);
      return -1;
    }

    compressionBufferSize = zInfo.total_out; // compressed size
    this->writeBytes(compressionBuffer, (long)compressionBufferSize);
    D(cout.flush() << "Packet::writeCompressedBytes():GZIP:numbytes:" << numBytes << ":compressedBytes:" << compressionBufferSize << "\n";)
    delete[] compressionBuffer;
    deflateEnd(&zInfo);
    return compressionBufferSize;
  }

  if (codec == COMPRESSION_SNAPPY)
  {
    unsigned long compressionBufferSize = snappy::MaxCompressedLength(numBytes);
    unsigned char* compressionBuffer = new unsigned char[compressionBufferSize];
    snappy::RawCompress((const char *)bytes, numBytes, (char *)compressionBuffer, &compressionBufferSize);
    this->writeBytes(compressionBuffer, (long)compressionBufferSize);
    D(cout.flush() << "Packet::writeCompressedBytes():SNAPPY:numbytes:" << numBytes << ":compressedBytes:" << compressionBufferSize << "\n";)
    delete[] compressionBuffer;
    return compressionBufferSize;
  }

  return -1; // invalid compression type
}

void Packet::resetForReading()
{
  this->head = buffer;
  this->size = readInt32();
}

int Packet::getSize(bool includeProtocolSizeFieldLength)
{
  if (includeProtocolSizeFieldLength) return this->size; // size instance variable is inclusive of size field length
  return (this->size - sizeof(int)); // protocol size field is exclusive of size field length
}

// The CRC32 functions make the following assumptions:
// 1 - head is pointing to an int32 crc field when beginCRC32() is called
// 2 - head is incremented past the crc field, and the next N bytes (to be CRC32'd) are written
// 3 - when endCRC32() is called, N bytes (this->head - this->crcHead) are CRC32'd, and the result written to the intial crc field (this->crcHead - sizeof(crc))
// 4 - the functions can't be used concurrently
void Packet::beginCRC32()
{
  D(cout.flush() << "Packet::beginCRC32()\n";)
  this->writeInt32(0); // will be updated @ endCRC32()
  this->crcHead = this->head;
}

int Packet::endCRC32()
{
  D(cout.flush() << "Packet::endCRC32()\n";)
  
  int crcLength = this->head - this->crcHead;
  D(cout.flush() << "Packet::endCRC32():crcLength:" << crcLength << "\n";)

  uLong initCrc = crc32(0L, Z_NULL, 0);
  uLong crc = crc32(initCrc, this->crcHead, crcLength);
  if (crc == initCrc)
  {
    E("Packet::endCRC32():error:updated crc matches initial (null) crc\n");
    return -1;
  }
  D(cout.flush() << "Packet::endCRC32():unsigned crc:" << crc << "\n";)
  int signedCrc = (int)crc;
  D(cout.flush() << "Packet::endCRC32():signed crc:" << signedCrc << "\n";)
  this->updateInt32(signedCrc, this->crcHead - sizeof(int));
  return signedCrc;
}

void Packet::seek(int numBytes)
{
  this->head += numBytes;
}

void Packet::writeToFile(string filepath)
{
  ofstream file;
  file.open(filepath.c_str(), ios::out | ios::app | ios::binary);
  if (file.is_open())
  {
    file.write((const char*)(this->buffer), this->size);
    file.close();
    D(cout.flush() << "Packet::writeToFile():" << this->size << " bytes written to " << filepath << "\n";)
  }
  else
  {
    E("Packet::writeToFile():error:unable to open file:\n");
    // errno output?
  }
}

}; // namespace LibKafka
