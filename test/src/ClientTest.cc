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
#include <Client.h>

using namespace std;

namespace {

  class ClientTest : public BaseTest {
    protected:

      ClientTest() { }
      virtual ~ClientTest() { }
      virtual void SetUp() { } 
      virtual void TearDown() { }
  };

  TEST_F(ClientTest, Constructor) {

    Client *c = new Client("wrigley.corp.adobe.com", 9092);
    EXPECT_NE(c, (void*)0);

    MetadataRequest *mr1 = createMetadataRequest(true);
    EXPECT_NE(mr1, (void*)0);
    MetadataResponse *mr2 = c->sendMetadataRequest(mr1);
    EXPECT_NE(mr2, (void*)0);
    if (mr1 != NULL) { cout << "ClientTest:MetadataRequest:\n" << *mr1; }
    if (mr2 != NULL) { cout << "ClientTest:MetadataResponse:\n" << *mr2; }
    
    ProduceRequest *pr1 = createProduceRequest();
    EXPECT_NE(pr1, (void*)0);
    ProduceResponse *pr2 = c->sendProduceRequest(pr1);
    EXPECT_NE(pr2, (void*)0);
    if (pr1 != NULL) { cout << "ClientTest:ProduceRequest:\n" << *pr1; }
    if (pr2 != NULL) { cout << "ClientTest:ProduceResponse:\n" << *pr2; }
    
    FetchRequest *fr1 = createFetchRequest();
    EXPECT_NE(fr1, (void*)0);
    FetchResponse *fr2 = c->sendFetchRequest(fr1);
    EXPECT_NE(fr2, (void*)0);
    if (fr1 != NULL) { cout << "ClientTest:FetchRequest:\n" << *fr1; }
    if (fr2 != NULL) { cout << "ClientTest:FetchResponse:\n" << *fr2; }

    fr2->packet->writeToFile("/tmp/fetchresponse.out");

    delete c;
    delete mr1;
    delete mr2;
    delete pr1;
    delete pr2;
    delete fr1;
    delete fr2;
  }

}  // namespace
