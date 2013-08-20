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

    Request *r1 = new Request(apiKey, apiVersion, correlationId, clientId);
    EXPECT_NE(r1, (void*)0);
    unsigned char * message = r1->toWireFormat();
    int size = r1->getWireFormatSize(true);
    EXPECT_EQ(r1->size(), size);

    Request *r2 = new Request(message);
    EXPECT_NE(r2, (void*)0);
    EXPECT_EQ(r2->size(), r1->size());
    EXPECT_EQ(r2->apiKey, r1->apiKey);
    EXPECT_EQ(r2->apiVersion, r1->apiVersion);
    EXPECT_EQ(r2->correlationId, r1->correlationId);
    EXPECT_EQ(r2->clientId, r1->clientId);

    delete r1;
    delete r2;
  }

}  // namespace
