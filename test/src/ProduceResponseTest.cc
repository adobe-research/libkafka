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

  class ProduceResponseTest : public BaseTest {
    protected:

      ProduceResponseTest() { }
      virtual ~ProduceResponseTest() { }
      virtual void SetUp() { } 
      virtual void TearDown() { }
  };

  TEST_F(ProduceResponseTest, Constructor) {
    
    ProduceResponse *pr1 = createProduceResponse();
    EXPECT_NE(pr1, (void*)0);
    unsigned char * message = pr1->toWireFormat();
    int size = pr1->getWireFormatSize(true);
    EXPECT_EQ(pr1->size(), size);

    ProduceResponse *pr2 = new ProduceResponse(message);

    EXPECT_NE(pr2, (void*)0);
    EXPECT_EQ(pr2->size(), pr1->size());
    EXPECT_EQ(pr2->produceResponseTopicArraySize, pr1->produceResponseTopicArraySize);
    for (int i=0; i<pr2->produceResponseTopicArraySize; i++) {
      EXPECT_EQ(*(pr2->produceResponseTopicArray[i]), *(pr1->produceResponseTopicArray[i]));
    }

    delete pr1;
    delete pr2;
  }

}  // namespace
