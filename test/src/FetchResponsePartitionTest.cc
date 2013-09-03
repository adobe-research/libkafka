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
#include <gtest/gtest.h>
#include "BaseTest.h"
#include <fetch/FetchResponsePartition.h>

namespace {

  class FetchResponsePartitionTest : public BaseTest {
    protected:

      FetchResponsePartitionTest() { }
      virtual ~FetchResponsePartitionTest() { }
      virtual void SetUp() { } 
      virtual void TearDown() { }
  };

  TEST_F(FetchResponsePartitionTest, Constructor) {

    FetchResponsePartition *frp1 = createFetchResponsePartition();
    EXPECT_NE(frp1, (void*)0);
    frp1->packet = new Packet();
    unsigned char * message = frp1->toWireFormat();
    int size = frp1->getWireFormatSize(true);
    EXPECT_EQ(frp1->packet->getSize(), size);

    frp1->packet->resetForReading();
    FetchResponsePartition *frp2 = new FetchResponsePartition(frp1->packet);

    EXPECT_NE(frp2, (void*)0);
    EXPECT_EQ(frp2->packet->getSize(), frp1->packet->getSize());
    EXPECT_EQ(frp2->partition, frp1->partition);
    EXPECT_EQ(frp2->errorCode, frp1->errorCode);
    EXPECT_EQ(frp2->highwaterMarkOffset, frp1->highwaterMarkOffset);
    EXPECT_EQ(frp2->messageSetSize, frp1->messageSetSize);

    delete frp1->packet;
    delete frp1;
    delete frp2;
  }

}  // namespace
