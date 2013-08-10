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
    RequestOrResponse *r1 = new RequestOrResponse();
    EXPECT_NE(r1, (void*)0);
    unsigned char * message = r1->toWireFormat();
    EXPECT_EQ(r1->size(), sizeof(int));

    RequestOrResponse *r2 = new RequestOrResponse(message);
    EXPECT_NE(r2, (void*)0);
    EXPECT_EQ(r2->size(), r1->size());
  }

}  // namespace
