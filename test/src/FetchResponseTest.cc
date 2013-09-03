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

  class FetchResponseTest : public BaseTest {
    protected:

      FetchResponseTest() { }
      virtual ~FetchResponseTest() { }
      virtual void SetUp() { } 
      virtual void TearDown() { }
  };

  TEST_F(FetchResponseTest, Constructor) {
    
    FetchResponse *fr1 = createFetchResponse();
    EXPECT_NE(fr1, (void*)0);
    unsigned char * message = fr1->toWireFormat();
    int size = fr1->getWireFormatSize(true);
    EXPECT_EQ(fr1->size(), size);

    FetchResponse *fr2 = new FetchResponse(message); 
    EXPECT_NE(fr2, (void*)0);
    EXPECT_EQ(fr2->size(), fr1->size());
    EXPECT_EQ(fr2->fetchResponseTopicArraySize, fr1->fetchResponseTopicArraySize);
    for (int i=0; i<fr2->fetchResponseTopicArraySize; i++) {
      EXPECT_EQ(*(fr2->fetchResponseTopicArray[i]), *(fr1->fetchResponseTopicArray[i]));
    }

    delete fr1;
    delete fr2;
  }

}  // namespace
