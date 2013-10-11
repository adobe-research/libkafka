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
#include <ApiConstants.h>

using namespace std;

namespace {

  class ApiConstantsTest : public BaseTest {
    protected:

      ApiConstantsTest() { }
      virtual ~ApiConstantsTest() { }
      virtual void SetUp() { }
      virtual void TearDown() { }
  };

  TEST_F(ApiConstantsTest, Constructor) {
    for (int i = ApiConstants::ERRORCODE_MINIMUM; i <= ApiConstants::ERRORCODE_MAXIMUM; i++)
    {
      const char *s = ApiConstants::getErrorString(i);
      EXPECT_NE(s, (char*)0);
    }
  }
  
}  // namespace
