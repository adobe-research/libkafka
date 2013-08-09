#include <string>
#include <gtest/gtest.h>
#include <BaseTest.h>
#include <Request.h>

namespace {

  class RequestTest : public BaseTest {
    protected:

      RequestTest() { }
      virtual ~RequestTest() { }
      virtual void SetUp() { } 
      virtual void TearDown() { }
  };

  TEST_F(RequestTest, Constructor) {
    short int apiKey = 1;
    short int apiVersion = 2;
    int correlationId = 3;
    string clientId = "testClientId";

    Request *r1 = new Request(apiKey, apiVersion, correlationId, clientId);
    EXPECT_NE(r1, (void*)0);
    unsigned char * message = r1->toWireFormat();
    EXPECT_EQ(r1->size, sizeof(int) + sizeof(short int) + sizeof(short int) + sizeof(int) + sizeof(short int) + clientId.length());

    Request *r2 = new Request(message);
    EXPECT_NE(r2, (void*)0);
    EXPECT_EQ(r2->size, r1->size);
    EXPECT_EQ(r2->apiKey, r1->apiKey);
    EXPECT_EQ(r2->apiVersion, r1->apiVersion);
    EXPECT_EQ(r2->correlationId, r1->correlationId);
    EXPECT_EQ(r2->clientId, r1->clientId);
  }

}  // namespace
