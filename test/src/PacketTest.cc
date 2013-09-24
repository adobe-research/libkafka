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

#include <Packet.h>
#include <gtest/gtest.h>
#include "BaseTest.h"

namespace {

  class PacketTest : public BaseTest {
    protected:

      PacketTest() { }
      virtual ~PacketTest() { }
      virtual void SetUp() { } 
      virtual void TearDown() { }
  };

  TEST_F(PacketTest, Constructor) {
    Packet *p1 = new Packet();
    EXPECT_NE(p1, (void*)0);
    p1->beginCRC32();
    p1->writeInt32(1);
    p1->writeInt32(2);
    p1->writeInt32(3);
    p1->endCRC32();
    unsigned char * message = p1->toWireFormat();
    EXPECT_EQ(p1->getSize(), sizeof(int) + (4 * sizeof(int))); // size + crc + 3 int32s

    Packet *p2 = new Packet(message);
    EXPECT_NE(p2, (void*)0);
    EXPECT_EQ(p2->getSize(), p1->getSize());
    int crc = p2->readInt32();
    EXPECT_EQ(crc, -1889021706); // calculated *signed int* CRC32 of 3 byte sequence [1,2,3] from above

    Packet *p3 = new Packet();
    EXPECT_NE(p3, (void*)0);
    int size = p3->writeCompressedBytes((unsigned char *)bytes, numBytes, Packet::COMPRESSION_GZIP);
    EXPECT_NE(size, -1);

    Packet *p4 = new Packet();
    EXPECT_NE(p4, (void*)0);
    size = p4->writeCompressedBytes((unsigned char*)bytes, numBytes, Packet::COMPRESSION_SNAPPY);
    EXPECT_NE(size, -1);

    //p2->writeToFile("/tmp/packet.out");

    delete p1;
    delete p2;
    delete p3;
    delete p4;
  }

}  // namespace
