#include <string>
#include <gtest/gtest.h>
#include "BaseTest.h"
#include <Packet.h>
#include <fetch/FetchPartition.h>

namespace {

  class FetchPartitionTest : public BaseTest {
    protected:

      FetchPartitionTest() { }
      virtual ~FetchPartitionTest() { }
      virtual void SetUp() { } 
      virtual void TearDown() { }
  };

  TEST_F(FetchPartitionTest, Constructor) {
    FetchPartition *fp1 = createFetchPartition();
    EXPECT_NE(fp1, (void*)0);
    fp1->packet = new Packet();
    unsigned char * message = fp1->toWireFormat();
    int size = fp1->getWireFormatSize(true);
    EXPECT_EQ(fp1->packet->getSize(), size);

    fp1->packet->resetForReading();
    FetchPartition *fp2 = new FetchPartition(fp1->packet);

    EXPECT_NE(fp2, (void*)0);
    EXPECT_EQ(fp2->packet->getSize(), fp1->packet->getSize());
    EXPECT_EQ(fp2->partition, fp1->partition);
    EXPECT_EQ(fp2->fetchOffset, fp1->fetchOffset);
    EXPECT_EQ(fp2->maxBytes, fp1->maxBytes);

    delete fp1->packet;
    delete fp1;
    delete fp2;
  }

}  // namespace
