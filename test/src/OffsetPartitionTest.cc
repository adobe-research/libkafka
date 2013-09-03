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
#include <Packet.h>
#include <offset/OffsetPartition.h>

namespace {

  class OffsetPartitionTest : public BaseTest {
    protected:

      OffsetPartitionTest() { }
      virtual ~OffsetPartitionTest() { }
      virtual void SetUp() { } 
      virtual void TearDown() { }
  };

  TEST_F(OffsetPartitionTest, Constructor) {
    OffsetPartition *op1 = createOffsetPartition();
    EXPECT_NE(op1, (void*)0);
    op1->packet = new Packet();
    unsigned char * message = op1->toWireFormat();
    int size = op1->getWireFormatSize(true);
    EXPECT_EQ(op1->packet->getSize(), size);

    op1->packet->resetForReading();
    OffsetPartition *op2 = new OffsetPartition(op1->packet);

    EXPECT_NE(op2, (void*)0);
    EXPECT_EQ(op2->packet->getSize(), op1->packet->getSize());
    EXPECT_EQ(op2->partition, op1->partition);
    EXPECT_EQ(op2->time, op1->time);
    EXPECT_EQ(op2->maxNumberOfOffsets, op1->maxNumberOfOffsets);

    delete op1->packet;
    delete op1;
    delete op2;
  }

}  // namespace
