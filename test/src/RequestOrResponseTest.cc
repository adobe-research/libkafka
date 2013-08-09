#include <RequestOrResponse.h>
#include <gtest/gtest.h>

namespace {

  class RequestOrResponseTest : public ::testing::Test {
    protected:

      RequestOrResponseTest() { }
      virtual ~RequestOrResponseTest() { }
      virtual void SetUp() { } 
      virtual void TearDown() { }

      // Objects declared here can be used by all tests in the test case for RequestOrResponseTest.
  };

  TEST_F(RequestOrResponseTest, Constructor) {
    int size = 21;
    RequestOrResponse *r = new RequestOrResponse((unsigned char *)&size);
    EXPECT_NE(r, (void*)0);
    EXPECT_EQ(r->size, size);
  }

}  // namespace
