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
#include "TestConfig.h"
#include <Client.h>
#include <Util.h>

using namespace std;

namespace {

  class ClientTest : public BaseTest {
    protected:

      Client *c;

      ClientTest() { }
      virtual ~ClientTest() { }
      virtual void SetUp() { c = new Client(TestConfig::CLIENT_BROKER_HOST, TestConfig::CLIENT_BROKER_PORT); } 
      virtual void TearDown() { delete c; }
  };

  TEST_F(ClientTest, Constructor) {
    EXPECT_NE(c, (void*)0);
  }
  
  TEST_F(ClientTest, MetadataRequest) {
    MetadataRequest *mr1 = createMetadataRequest(true);
    EXPECT_NE(mr1, (void*)0);
    MetadataResponse *mr2 = c->sendMetadataRequest(mr1);
    EXPECT_NE(mr2, (void*)0);
    EXPECT_EQ(mr2->hasErrorCode(), false);
    if (mr1 != NULL) { cout << "ClientTest:MetadataRequest:\n" << *mr1; delete mr1; }
    if (mr2 != NULL) { cout << "ClientTest:MetadataResponse:\n" << *mr2; delete mr2; }
  }
    
  TEST_F(ClientTest, ProduceRequest) {
    ProduceRequest *pr1 = createProduceRequest();
    EXPECT_NE(pr1, (void*)0);
    ProduceResponse *pr2 = c->sendProduceRequest(pr1);
    EXPECT_NE(pr2, (void*)0);
    EXPECT_EQ(pr2->hasErrorCode(), false);
    if (pr1 != NULL) { cout << "ClientTest:ProduceRequest:\n" << *pr1; delete pr1; }
    if (pr2 != NULL) { cout << "ClientTest:ProduceResponse:\n" << *pr2; delete pr2; }
  }
    
  TEST_F(ClientTest, ProduceRequestWithGZIP) {
    ProduceRequest *pr1 = createProduceRequest();
    pr1->setCompression(ApiConstants::MESSAGE_COMPRESSION_GZIP);
    cout << *pr1;
    EXPECT_NE(pr1, (void*)0);
    ProduceResponse *pr2 = c->sendProduceRequest(pr1);
    EXPECT_NE(pr2, (void*)0);
    EXPECT_EQ(pr2->hasErrorCode(), false);
    if (pr1 != NULL) { cout << "ClientTest:ProduceRequest:Compression:GZIP\n" << *pr1; delete pr1; }
    if (pr2 != NULL) { cout << "ClientTest:ProduceResponse:Compression:GZIP\n" << *pr2; delete pr2; }
  }
    
  TEST_F(ClientTest, ProduceRequestWithSnappy) {
    ProduceRequest *pr1 = createProduceRequest();
    pr1->setCompression(ApiConstants::MESSAGE_COMPRESSION_SNAPPY);
    EXPECT_NE(pr1, (void*)0);
    ProduceResponse *pr2 = c->sendProduceRequest(pr1);
    EXPECT_NE(pr2, (void*)0);
    EXPECT_EQ(pr2->hasErrorCode(), false);
    if (pr1 != NULL) { cout << "ClientTest:ProduceRequest:Compression:Snappy\n" << *pr1; delete pr1; }
    if (pr2 != NULL) { cout << "ClientTest:ProduceResponse:Compression:Snappy\n" << *pr2; delete pr2; }
  }

  TEST_F(ClientTest, OffsetRequest) {
    OffsetRequest *or1 = createOffsetRequest();
    EXPECT_NE(or1, (void*)0);
    OffsetResponse *or2 = c->sendOffsetRequest(or1);
    EXPECT_NE(or2, (void*)0);
    EXPECT_EQ(or2->hasErrorCode(), false);
    if (or1 != NULL) { cout << "ClientTest:OffsetRequest:\n" << *or1; delete or1; }
    if (or2 != NULL) { cout << "ClientTest:OffsetResponse:\n" << *or2; delete or2; }
  }
    
  TEST_F(ClientTest, FetchRequest) {
    // get offset for fetch request
    OffsetRequest *or1 = createOffsetRequest();
    EXPECT_NE(or1, (void*)0);
    OffsetResponse *or2 = c->sendOffsetRequest(or1);
    EXPECT_NE(or2, (void*)0);
    EXPECT_EQ(or2->hasErrorCode(), false);

    long startOffset = or2->offsetResponseTopicArray[0]->subBlockArray[0]->offsetArray[1]; // should be first available msg
    FetchRequest *fr1 = createFetchRequest(startOffset);
    EXPECT_NE(fr1, (void*)0);
    FetchResponse *fr2 = c->sendFetchRequest(fr1);
    EXPECT_NE(fr2, (void*)0);
    EXPECT_EQ(fr2->hasErrorCode(), false);
    //fr2->packet->writeToFile("/tmp/fetchresponse.out");

    if (or1 != NULL) { cout << "ClientTest:OffsetRequest:\n" << *or1; delete or1; }
    if (or2 != NULL) { cout << "ClientTest:OffsetResponse:\n" << *or2; delete or2; }
    if (fr1 != NULL) { cout << "ClientTest:FetchRequest:\n" << *fr1; delete fr1; }
    if (fr2 != NULL) { cout << "ClientTest:FetchResponse:\n" << *fr2; delete fr2; }
  }

}  // namespace
