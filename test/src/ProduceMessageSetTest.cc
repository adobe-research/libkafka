#include <string>
#include <gtest/gtest.h>
#include "BaseTest.h"
#include <produce/ProduceMessageSet.h>

namespace {

  class ProduceMessageSetTest : public BaseTest {
    protected:

      ProduceMessageSetTest() { }
      virtual ~ProduceMessageSetTest() { }
      virtual void SetUp() { } 
      virtual void TearDown() { }
  };

  TEST_F(ProduceMessageSetTest, Constructor) {

    ProduceMessageSet *pm1 = createProduceMessageSet();
    EXPECT_NE(pm1, (void*)0);
    pm1->packet = new Packet();
    unsigned char * message = pm1->toWireFormat();
    int size = pm1->getWireFormatSize(true);
    EXPECT_EQ(pm1->packet->getSize(), size);

    pm1->packet->resetForReading();
    ProduceMessageSet *pm2 = new ProduceMessageSet(pm1->packet);

    EXPECT_NE(pm2, (void*)0);
    EXPECT_EQ(pm2->packet->getSize(), pm1->packet->getSize());
    EXPECT_EQ(pm2->partition, pm1->partition);
    EXPECT_EQ(pm2->messageSetSize, pm1->messageSetSize);
    EXPECT_EQ(*(pm2->messageSet), *(pm1->messageSet));

    delete pm1->packet;
    delete pm1;
    delete pm2;
  }

}  // namespace
