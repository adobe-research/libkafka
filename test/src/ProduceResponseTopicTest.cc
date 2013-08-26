#include <string>
#include <gtest/gtest.h>
#include "BaseTest.h"
#include <Packet.h>
#include <produce/ProduceResponseTopic.h>

namespace {

  class ProduceResponseTopicTest : public BaseTest {
    protected:

      ProduceResponseTopicTest() { }
      virtual ~ProduceResponseTopicTest() { }
      virtual void SetUp() { } 
      virtual void TearDown() { }
  };

  TEST_F(ProduceResponseTopicTest, Constructor) {
    ProduceResponseTopic *pt1 = createProduceResponseTopic();
    EXPECT_NE(pt1, (void*)0);
    pt1->packet = new Packet();
    unsigned char * message = pt1->toWireFormat();
    int size = pt1->getWireFormatSize(true);
    EXPECT_EQ(pt1->packet->getSize(), size);

    pt1->packet->resetForReading();
    ProduceResponseTopic *pt2 = new ProduceResponseTopic(pt1->packet);

    EXPECT_NE(pt2, (void*)0);
    EXPECT_EQ(pt2->packet->getSize(), pt1->packet->getSize());
    EXPECT_EQ(pt2->topicName, pt1->topicName);
    EXPECT_EQ(pt2->produceResponsePartitionArraySize, pt1->produceResponsePartitionArraySize);
    for (int i=0; i<pt2->produceResponsePartitionArraySize; i++) {
      EXPECT_EQ(*(pt2->produceResponsePartitionArray[i]), *(pt1->produceResponsePartitionArray[i]));
    }

    delete pt1->packet;
    delete pt1;
    delete pt2;
  }

}  // namespace
