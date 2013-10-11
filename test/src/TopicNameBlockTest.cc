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
#include <TopicNameBlock.h>
#include <fetch/FetchPartition.h>

namespace {

  class TopicNameBlockTest : public BaseTest {
    protected:

      TopicNameBlockTest() { }
      virtual ~TopicNameBlockTest() { }
      virtual void SetUp() { } 
      virtual void TearDown() { }
  };

  TEST_F(TopicNameBlockTest, Constructor) {
    TopicNameBlock<FetchPartition> *tnb1 = createTopicNameBlock();
    EXPECT_NE(tnb1, (void*)0);
    tnb1->packet = new Packet();
    unsigned char * message = tnb1->toWireFormat();
    int size = tnb1->getWireFormatSize(true);
    EXPECT_EQ(tnb1->packet->getSize(), size);

    tnb1->packet->resetForReading();
    TopicNameBlock<FetchPartition> *tnmb2 = new TopicNameBlock<FetchPartition>(tnb1->packet);

    EXPECT_NE(tnmb2, (void*)0);
    EXPECT_EQ(tnmb2->packet->getSize(), tnb1->packet->getSize());
    EXPECT_EQ(tnmb2->topicName, tnb1->topicName);
    EXPECT_EQ(tnmb2->subBlockArraySize, tnb1->subBlockArraySize);
    for (int i=0; i<tnmb2->subBlockArraySize; i++) {
      EXPECT_EQ(*(tnmb2->subBlockArray[i]), *(tnb1->subBlockArray[i]));
    }
    cout << *tnb1;

    delete tnb1->packet;
    delete tnb1;
    delete tnmb2;
  }

}  // namespace
