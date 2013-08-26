#include <string>
#include <gtest/gtest.h>
#include "BaseTest.h"
#include <metadata/Broker.h>

namespace {

  class BrokerTest : public BaseTest {
    protected:

      BrokerTest() { }
      virtual ~BrokerTest() { }
      virtual void SetUp() { } 
      virtual void TearDown() { }
  };

  TEST_F(BrokerTest, Constructor) {

    Broker *b1 = createBroker();
    EXPECT_NE(b1, (void*)0);
    b1->packet = new Packet();
    unsigned char * message = b1->toWireFormat();
    int size = b1->getWireFormatSize(true);
    EXPECT_EQ(b1->packet->getSize(), size);

    b1->packet->resetForReading();
    Broker *b2 = new Broker(b1->packet);

    EXPECT_NE(b2, (void*)0);
    EXPECT_EQ(b2->packet->getSize(), b1->packet->getSize());
    EXPECT_EQ(b2->nodeId, b1->nodeId);
    EXPECT_EQ(b2->host, b1->host);
    EXPECT_EQ(b2->port, b1->port);

    delete b1->packet;
    delete b1;
    delete b2;
  }

}  // namespace
