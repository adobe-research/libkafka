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
#include <metadata/TopicMetadata.h>
#include <metadata/PartitionMetadata.h>

namespace {

  class TopicMetadataTest : public BaseTest {
    protected:

      TopicMetadataTest() { }
      virtual ~TopicMetadataTest() { }
      virtual void SetUp() { } 
      virtual void TearDown() { }
  };

  TEST_F(TopicMetadataTest, Constructor) {
    TopicMetadata *tm1 = createTopicMetadata();
    EXPECT_NE(tm1, (void*)0);
    tm1->packet = new Packet();
    unsigned char * message = tm1->toWireFormat();
    int size = tm1->getWireFormatSize(true);
    EXPECT_EQ(tm1->packet->getSize(), size);

    tm1->packet->resetForReading();
    TopicMetadata *tm2 = new TopicMetadata(tm1->packet);

    EXPECT_NE(tm2, (void*)0);
    EXPECT_EQ(tm2->packet->getSize(), tm1->packet->getSize());
    EXPECT_EQ(tm2->topicErrorCode, tm1->topicErrorCode);
    EXPECT_EQ(tm2->topicName, tm1->topicName);
    EXPECT_EQ(tm2->partitionMetadataArraySize, tm1->partitionMetadataArraySize);
    for (int i=0; i<tm2->partitionMetadataArraySize; i++) {
      EXPECT_EQ(*(tm2->partitionMetadataArray[i]), *(tm1->partitionMetadataArray[i]));
    }

    delete tm1->packet;
    delete tm1;
    delete tm2;
  }

}  // namespace
