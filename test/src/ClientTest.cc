#include <string>
#include <gtest/gtest.h>
#include <BaseTest.h>
#include <Client.h>

namespace {

  class ClientTest : public BaseTest {
    protected:

      ClientTest() { }
      virtual ~ClientTest() { }
      virtual void SetUp() { } 
      virtual void TearDown() { }
  };

  TEST_F(ClientTest, Constructor) {

    Client *c = new Client("wrigley.corp.adobe.com", 9092);
    EXPECT_NE(c, (void*)0);
    MetadataRequest *mr1 = createMetadataRequest(true);
    EXPECT_NE(mr1, (void*)0);
    MetadataResponse *mr2 = c->sendMetadataRequest(mr1);
    EXPECT_NE(mr2, (void*)0);
    delete c;
  }

}  // namespace
