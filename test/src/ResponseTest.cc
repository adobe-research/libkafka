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
    init_packet();

    int size = 21;
    int correlationId = 3;

    write_int32(size);
    write_int32(correlationId);

    Response *r = new Response((unsigned char *)&packet);
    EXPECT_NE(r, (void*)0);
    EXPECT_EQ(r->size, size);
    EXPECT_EQ(r->correlationId, correlationId);
  }

}  // namespace
