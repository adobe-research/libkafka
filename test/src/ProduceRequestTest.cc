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

namespace {

  class ProduceRequestTest : public BaseTest {
    protected:

      ProduceRequest *pr1, *pr2, *pr3;
      unsigned char *message;

      ProduceRequestTest() { }
      virtual ~ProduceRequestTest() { }
      virtual void SetUp() { } 
      virtual void TearDown() { }
  };

  TEST_F(ProduceRequestTest, Constructor) {
    pr1 = createProduceRequest();
    EXPECT_NE(pr1, (void*)0);
    delete pr1;
  }
  
  TEST_F(ProduceRequestTest, ToWireFormat) {
    pr1 = createProduceRequest();
    message = pr1->toWireFormat();
    int size = pr1->getWireFormatSize(true);
    EXPECT_EQ(pr1->size(), size);
    delete pr1;
  }
  
  TEST_F(ProduceRequestTest, Deserialization) {
    pr1 = createProduceRequest();
    message = pr1->toWireFormat();
    pr2 = new ProduceRequest(message);
    EXPECT_NE(pr2, (void*)0);
    EXPECT_EQ(pr2->size(), pr1->size());
    EXPECT_EQ(pr2->requiredAcks, pr1->requiredAcks);
    EXPECT_EQ(pr2->timeout, pr1->timeout);
    for (int i=0; i<pr2->produceTopicArraySize; i++) {
      EXPECT_EQ(*(pr2->produceTopicArray[i]), *(pr1->produceTopicArray[i]));
    }
    delete pr1;
    delete pr2;
  }
  
  TEST_F(ProduceRequestTest, CompressionGZIP) {
    pr3 = createProduceRequest();
    EXPECT_NE(pr3, (void*)0);
    pr3->setCompression(ApiConstants::MESSAGE_COMPRESSION_GZIP);
    message = pr3->toWireFormat();
    int size = pr3->getWireFormatSize(true);
    EXPECT_EQ(pr3->size(), size);
    delete pr3;
  }
  
  TEST_F(ProduceRequestTest, CompressionSnappy) {
    pr3 = createProduceRequest();
    EXPECT_NE(pr3, (void*)0);
    pr3->setCompression(ApiConstants::MESSAGE_COMPRESSION_SNAPPY);
    message = pr3->toWireFormat();
    int size = pr3->getWireFormatSize(true);
    EXPECT_EQ(pr3->size(), size);
    delete pr3;
  }

}  // namespace
