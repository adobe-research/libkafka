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
    EXPECT_EQ(0, 0);
  }

  TEST_F(RequestOrResponseTest, Parse) {
    EXPECT_EQ(0, 0);
  }

}  // namespace
