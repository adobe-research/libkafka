#include <string>
#include <gtest/gtest.h>
#include <Request.h>

namespace {

  class RequestTest : public ::testing::Test {
    protected:

      RequestTest() { }
      virtual ~RequestTest() { }
      virtual void SetUp() { } 
      virtual void TearDown() { }

      // Objects declared here can be used by all tests in the test case for RequestTest.
  };

  TEST_F(RequestTest, Constructor) {
    // build test packet
    unsigned char buffer[256];
    unsigned char *head = (unsigned char *)&buffer;

    int size = 21;
    short int apiKey = 1;
    short int apiVersion = 2;
    int correlationId = 3;
    string clientId = "testClientId";
    short int clientIdLength = (short int)clientId.length();

    memcpy(head, &size, sizeof(int));
    head += sizeof(int);
    memcpy(head, &apiKey, sizeof(short int));
    head += sizeof(short int);
    memcpy(head, &apiVersion, sizeof(short int));
    head += sizeof(short int);
    memcpy(head, &correlationId, sizeof(int));
    head += sizeof(int);
    memcpy(head, &clientIdLength, sizeof(short int));
    head += sizeof(short int);
    memcpy(head, clientId.data(), clientIdLength);

    Request *r = new Request((unsigned char *)&buffer);
    EXPECT_NE(r, (void*)0);
    EXPECT_EQ(r->size, size);
    EXPECT_EQ(r->apiKey, apiKey);
    EXPECT_EQ(r->apiVersion, apiVersion);
    EXPECT_EQ(r->correlationId, correlationId);
    EXPECT_EQ(r->clientId, clientId);
  }

}  // namespace
