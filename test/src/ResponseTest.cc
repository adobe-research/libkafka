#include <string>
#include <gtest/gtest.h>
#include <BaseTest.h>
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
  }

}  // namespace
