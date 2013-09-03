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
#include <produce/ProduceResponsePartition.h>

namespace {

  class ProduceResponsePartitionTest : public BaseTest {
    protected:

      ProduceResponsePartitionTest() { }
      virtual ~ProduceResponsePartitionTest() { }
      virtual void SetUp() { } 
      virtual void TearDown() { }
  };

  TEST_F(ProduceResponsePartitionTest, Constructor) {

    ProduceResponsePartition *pm1 = createProduceResponsePartition();
    EXPECT_NE(pm1, (void*)0);
    pm1->packet = new Packet();
    unsigned char * message = pm1->toWireFormat();
    int size = pm1->getWireFormatSize(true);
    EXPECT_EQ(pm1->packet->getSize(), size);

    pm1->packet->resetForReading();
    ProduceResponsePartition *pm2 = new ProduceResponsePartition(pm1->packet);

    EXPECT_NE(pm2, (void*)0);
    EXPECT_EQ(pm2->packet->getSize(), pm1->packet->getSize());
    EXPECT_EQ(pm2->partition, pm1->partition);
    EXPECT_EQ(pm2->errorCode, pm1->errorCode);
    EXPECT_EQ(pm2->offset, pm1->offset);

    delete pm1->packet;
    delete pm1;
    delete pm2;
  }

}  // namespace
