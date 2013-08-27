#include <string>
#include <gtest/gtest.h>
#include "BaseTest.h"
#include <Client.h>

using namespace std;

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
    cout << "ClientTest:MetadataRequest:\n" << *mr1;
    cout << "ClientTest:MetadataResponse:\n" << *mr2;
    
    ProduceRequest *pr1 = createProduceRequest();
    EXPECT_NE(pr1, (void*)0);
    ProduceResponse *pr2 = c->sendProduceRequest(pr1);
    EXPECT_NE(pr2, (void*)0);
    cout << "ClientTest:ProduceRequest:\n" << *pr1;
    cout << "ClientTest:ProduceResponse:\n" << *pr2;

    delete c;
    delete mr1;
    delete mr2;
    delete pr1;
    delete pr2;
  }

}  // namespace
