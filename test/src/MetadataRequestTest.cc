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
#include <metadata/MetadataRequest.h>

namespace {

  class MetadataRequestTest : public BaseTest {
    protected:

      MetadataRequestTest() { }
      virtual ~MetadataRequestTest() { }
      virtual void SetUp() { } 
      virtual void TearDown() { }
  };

  TEST_F(MetadataRequestTest, Constructor) {

    MetadataRequest *mr1 = createMetadataRequest();
    EXPECT_NE(mr1, (void*)0);
    unsigned char * message = mr1->toWireFormat();
    int size = mr1->getWireFormatSize(true);
    EXPECT_EQ(mr1->size(), size);

    MetadataRequest *mr2 = new MetadataRequest(message);

    EXPECT_NE(mr2, (void*)0);
    EXPECT_EQ(mr2->size(), mr1->size());
    EXPECT_EQ(mr2->apiKey, mr1->apiKey);
    EXPECT_EQ(mr2->apiVersion, mr1->apiVersion);
    EXPECT_EQ(mr2->correlationId, mr1->correlationId);
    EXPECT_EQ(mr2->clientId, mr1->clientId);
    EXPECT_EQ(mr2->topicNameArraySize, mr1->topicNameArraySize);
    for (int i=0; i<mr2->topicNameArraySize; i++) {
      EXPECT_EQ(mr2->topicNameArray[i], mr1->topicNameArray[i]);
    }
    cout << *mr2;

    delete mr1;
    delete mr2;
  }

}  // namespace
