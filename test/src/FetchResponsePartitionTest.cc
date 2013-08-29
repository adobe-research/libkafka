#include <string>
#include <gtest/gtest.h>
#include "BaseTest.h"
#include <fetch/FetchResponsePartition.h>

namespace {

  class FetchResponsePartitionTest : public BaseTest {
    protected:

      FetchResponsePartitionTest() { }
      virtual ~FetchResponsePartitionTest() { }
      virtual void SetUp() { } 
      virtual void TearDown() { }
  };

  TEST_F(FetchResponsePartitionTest, Constructor) {

    FetchResponsePartition *frp1 = createFetchResponsePartition();
    EXPECT_NE(frp1, (void*)0);
    frp1->packet = new Packet();
    unsigned char * message = frp1->toWireFormat();
    int size = frp1->getWireFormatSize(true);
    EXPECT_EQ(frp1->packet->getSize(), size);

    frp1->packet->resetForReading();
    FetchResponsePartition *frp2 = new FetchResponsePartition(frp1->packet);

    EXPECT_NE(frp2, (void*)0);
    EXPECT_EQ(frp2->packet->getSize(), frp1->packet->getSize());
    EXPECT_EQ(frp2->partition, frp1->partition);
    EXPECT_EQ(frp2->errorCode, frp1->errorCode);
    EXPECT_EQ(frp2->highwaterMarkOffset, frp1->highwaterMarkOffset);
    EXPECT_EQ(frp2->messageSetSize, frp1->messageSetSize);

    delete frp1->packet;
    delete frp1;
    delete frp2;
  }

}  // namespace
