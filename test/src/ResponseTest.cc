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
    int size = 21;
    int correlationId = 3;

    Response *r1 = new Response(correlationId);
    EXPECT_NE(r1, (void*)0);
    unsigned char * message = r1->toWireFormat();
    EXPECT_EQ(r1->size, sizeof(int) + sizeof(int));

    Response *r2 = new Response(message);
    EXPECT_NE(r2, (void*)0);
    EXPECT_EQ(r2->size, r1->size);
    EXPECT_EQ(r2->correlationId, r1->correlationId);
  }

}  // namespace
