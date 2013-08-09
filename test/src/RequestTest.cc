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
    init_packet();

    int size = 21;
    short int apiKey = 1;
    short int apiVersion = 2;
    int correlationId = 3;
    string clientId = "testClientId";

    write_int32(size);
    write_int16(apiKey);
    write_int16(apiVersion);
    write_int32(correlationId);
    write_string(clientId);

    Request *r = new Request((unsigned char *)&packet);
    EXPECT_NE(r, (void*)0);
    EXPECT_EQ(r->size, size);
    EXPECT_EQ(r->apiKey, apiKey);
    EXPECT_EQ(r->apiVersion, apiVersion);
    EXPECT_EQ(r->correlationId, correlationId);
    EXPECT_EQ(r->clientId, clientId);
  }

}  // namespace
