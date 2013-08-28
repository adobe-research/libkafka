#include <string>
#include <gtest/gtest.h>
#include "BaseTest.h"
#include <Packet.h>
#include <TopicNameBlock.h>
#include <fetch/FetchPartition.h>

namespace {

  class TopicNameBlockTest : public BaseTest {
    protected:

      TopicNameBlockTest() { }
      virtual ~TopicNameBlockTest() { }
      virtual void SetUp() { } 
      virtual void TearDown() { }
  };

  TEST_F(TopicNameBlockTest, Constructor) {
    TopicNameBlock<FetchPartition> *tnb1 = createTopicNameBlock();
    EXPECT_NE(tnb1, (void*)0);
    tnb1->packet = new Packet();
    unsigned char * message = tnb1->toWireFormat();
    int size = tnb1->getWireFormatSize(true);
    EXPECT_EQ(tnb1->packet->getSize(), size);

    tnb1->packet->resetForReading();
    TopicNameBlock<FetchPartition> *tnmb2 = new TopicNameBlock<FetchPartition>(tnb1->packet);

    EXPECT_NE(tnmb2, (void*)0);
    EXPECT_EQ(tnmb2->packet->getSize(), tnb1->packet->getSize());
    EXPECT_EQ(tnmb2->topicName, tnb1->topicName);
    EXPECT_EQ(tnmb2->subBlockArraySize, tnb1->subBlockArraySize);
    for (int i=0; i<tnmb2->subBlockArraySize; i++) {
      EXPECT_EQ(*(tnmb2->subBlockArray[i]), *(tnb1->subBlockArray[i]));
    }

    delete tnb1->packet;
    delete tnb1;
    delete tnmb2;
  }

}  // namespace
