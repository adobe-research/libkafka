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
#include <fetch/FetchPartition.h>

namespace {

  class FetchPartitionTest : public BaseTest {
    protected:

      FetchPartitionTest() { }
      virtual ~FetchPartitionTest() { }
      virtual void SetUp() { } 
      virtual void TearDown() { }
  };

  TEST_F(FetchPartitionTest, Constructor) {
    FetchPartition *fp1 = createFetchPartition();
    EXPECT_NE(fp1, (void*)0);
    fp1->packet = new Packet();
    unsigned char * message = fp1->toWireFormat();
    int size = fp1->getWireFormatSize(true);
    EXPECT_EQ(fp1->packet->getSize(), size);

    fp1->packet->resetForReading();
    FetchPartition *fp2 = new FetchPartition(fp1->packet);

    EXPECT_NE(fp2, (void*)0);
    EXPECT_EQ(fp2->packet->getSize(), fp1->packet->getSize());
    EXPECT_EQ(fp2->partition, fp1->partition);
    EXPECT_EQ(fp2->fetchOffset, fp1->fetchOffset);
    EXPECT_EQ(fp2->maxBytes, fp1->maxBytes);
    EXPECT_EQ(fp2->hasErrorCode(), false);

    delete fp1->packet;
    delete fp1;
    delete fp2;
  }

}  // namespace
