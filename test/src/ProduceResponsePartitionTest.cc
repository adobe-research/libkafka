#include <string>
#include <gtest/gtest.h>
#include "BaseTest.h"
#include <produce/ProduceResponsePartition.h>

namespace {

  class ProduceResponsePartitionTest : public BaseTest {
    protected:

      ProduceResponsePartitionTest() { }
      virtual ~ProduceResponsePartitionTest() { }
      virtual void SetUp() { } 
      virtual void TearDown() { }
  };

  TEST_F(ProduceResponsePartitionTest, Constructor) {

    ProduceResponsePartition *pm1 = createProduceResponsePartition();
    EXPECT_NE(pm1, (void*)0);
    pm1->packet = new Packet();
    unsigned char * message = pm1->toWireFormat();
    int size = pm1->getWireFormatSize(true);
    EXPECT_EQ(pm1->packet->getSize(), size);

    pm1->packet->resetForReading();
    ProduceResponsePartition *pm2 = new ProduceResponsePartition(pm1->packet);

    EXPECT_NE(pm2, (void*)0);
    EXPECT_EQ(pm2->packet->getSize(), pm1->packet->getSize());
    EXPECT_EQ(pm2->partition, pm1->partition);
    EXPECT_EQ(pm2->errorCode, pm1->errorCode);
    EXPECT_EQ(pm2->offset, pm1->offset);

    delete pm1->packet;
    delete pm1;
    delete pm2;
  }

}  // namespace
