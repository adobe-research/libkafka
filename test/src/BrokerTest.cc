#include <string>
#include <gtest/gtest.h>
#include <BaseTest.h>
#include <Broker.h>

namespace {

  class BrokerTest : public BaseTest {
    protected:

      BrokerTest() { }
      virtual ~BrokerTest() { }
      virtual void SetUp() { } 
      virtual void TearDown() { }
  };

  TEST_F(BrokerTest, Constructor) {
    int nodeId = 1;
    string host = string("testhost");
    int port = 8000;

    Broker *b1 = new Broker(nodeId, host, port);
    EXPECT_NE(b1, (void*)0);
    b1->packet = new Packet();
    unsigned char * message = b1->toWireFormat();
    int size = sizeof(int) + sizeof(int) + sizeof(int) + sizeof(short int) + host.length();
    EXPECT_EQ(b1->packet->size, size);

    b1->packet->resetForReading();
    Broker *b2 = new Broker(b1->packet);

    EXPECT_NE(b2, (void*)0);
    EXPECT_EQ(b2->packet->size, b1->packet->size);
    EXPECT_EQ(b2->nodeId, b1->nodeId);
    EXPECT_EQ(b2->host, b1->host);
    EXPECT_EQ(b2->port, b1->port);
  }

}  // namespace
