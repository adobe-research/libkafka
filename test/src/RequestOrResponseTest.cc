#include <RequestOrResponse.h>
#include <gtest/gtest.h>
#include <BaseTest.h>

namespace {

  class RequestOrResponseTest : public BaseTest {
    protected:

      RequestOrResponseTest() { }
      virtual ~RequestOrResponseTest() { }
      virtual void SetUp() { } 
      virtual void TearDown() { }
  };

  TEST_F(RequestOrResponseTest, Constructor) {
    int size = 21;
    init_packet();
    write_int32(size);
    RequestOrResponse *r = new RequestOrResponse((unsigned char *)&packet);
    EXPECT_NE(r, (void*)0);
    EXPECT_EQ(r->size, size);
  }

}  // namespace
