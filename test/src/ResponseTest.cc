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
#include <Response.h>

namespace {

  class ResponseTest : public BaseTest {
    protected:

      ResponseTest() { }
      virtual ~ResponseTest() { }
      virtual void SetUp() { } 
      virtual void TearDown() { }
  };

  TEST_F(ResponseTest, Constructor) {

    Response *r1 = new Response(correlationId);
    EXPECT_NE(r1, (void*)0);
    unsigned char * message = r1->toWireFormat();
    int size = r1->getWireFormatSize(true);
    EXPECT_EQ(r1->size(), size);

    Response *r2 = new Response(message);
    EXPECT_NE(r2, (void*)0);
    EXPECT_EQ(r2->size(), r1->size());
    EXPECT_EQ(r2->correlationId, r1->correlationId);

    delete r1;
    delete r2;
  }

}  // namespace
