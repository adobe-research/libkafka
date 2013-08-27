#include <string>
#include <gtest/gtest.h>
#include "BaseTest.h"
#include <Packet.h>
#include <fetch/FetchTopic.h>

namespace {

  class FetchTopicTest : public BaseTest {
    protected:

      FetchTopicTest() { }
      virtual ~FetchTopicTest() { }
      virtual void SetUp() { } 
      virtual void TearDown() { }
  };

  TEST_F(FetchTopicTest, Constructor) {
    FetchTopic *pt1 = createFetchTopic();
    EXPECT_NE(pt1, (void*)0);
    pt1->packet = new Packet();
    unsigned char * message = pt1->toWireFormat();
    int size = pt1->getWireFormatSize(true);
    EXPECT_EQ(pt1->packet->getSize(), size);

    pt1->packet->resetForReading();
    FetchTopic *pt2 = new FetchTopic(pt1->packet);

    EXPECT_NE(pt2, (void*)0);
    EXPECT_EQ(pt2->packet->getSize(), pt1->packet->getSize());
    EXPECT_EQ(pt2->topicName, pt1->topicName);
    EXPECT_EQ(pt2->fetchPartitionArraySize, pt1->fetchPartitionArraySize);
    for (int i=0; i<pt2->fetchPartitionArraySize; i++) {
      EXPECT_EQ(*(pt2->fetchPartitionArray[i]), *(pt1->fetchPartitionArray[i]));
    }

    delete pt1->packet;
    delete pt1;
    delete pt2;
  }

}  // namespace
