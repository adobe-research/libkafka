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
#include <MessageSet.h>
#include <metadata/PartitionMetadata.h>

namespace {

  class MessageSetTest : public BaseTest {
    protected:

      MessageSetTest() { }
      virtual ~MessageSetTest() { }
      virtual void SetUp() { } 
      virtual void TearDown() { }
  };

  TEST_F(MessageSetTest, Constructor) {
    MessageSet *m1 = createMessageSet();
    cout << *m1;
    EXPECT_NE(m1, (void*)0);
    m1->packet = new Packet();
    unsigned char * message = m1->toWireFormat();
    int size = m1->getWireFormatSize(true);
    EXPECT_EQ(m1->packet->getSize(), size);

    int messageSetSize = m1->getWireFormatSize(false);
    m1->packet->resetForReading();
    MessageSet *m2 = new MessageSet(messageSetSize, m1->packet);
    cout << *m2;

    EXPECT_NE(m2, (void*)0);
    EXPECT_EQ(m2->packet->getSize(), m1->packet->getSize());
    EXPECT_EQ(m2->messageSetSize, m1->messageSetSize);
    // test individual messages are similar

    delete m1->packet;
    delete m1;
    delete m2;
  }

}  // namespace
