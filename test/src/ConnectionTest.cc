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
#include <Connection.h>

namespace {

  class ConnectionTest : public BaseTest {
    protected:

      ConnectionTest() { }
      virtual ~ConnectionTest() { }
      virtual void SetUp() { } 
      virtual void TearDown() { }
  };

  TEST_F(ConnectionTest, Constructor) {

    Connection *c = new Connection("www.google.com", 80);
    EXPECT_NE(c, (void*)0);
    int status = c->open();
    EXPECT_NE(status, Connection::OPEN_CONNECTION_ERROR);
    string request = "GET / HTTP/1.1\nhost: www.google.com\n\n";
    int length = request.length();
    int numBytesSent = c->write(length, (unsigned char*)(request.c_str()));
    EXPECT_EQ(numBytesSent, length);
    unsigned char* buffer = new unsigned char[1024];
    int numBytesReceived = c->read(1024, buffer);
    EXPECT_GT(numBytesReceived, 0);

    c->close();
    delete buffer;
    delete c;
  }

}  // namespace
