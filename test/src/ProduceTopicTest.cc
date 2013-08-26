#include <string>
#include <gtest/gtest.h>
#include "BaseTest.h"
#include <Packet.h>
#include <produce/ProduceTopic.h>

namespace {

  class ProduceTopicTest : public BaseTest {
    protected:

      ProduceTopicTest() { }
      virtual ~ProduceTopicTest() { }
      virtual void SetUp() { } 
      virtual void TearDown() { }
  };

  TEST_F(ProduceTopicTest, Constructor) {
    ProduceTopic *pt1 = createProduceTopic();
    EXPECT_NE(pt1, (void*)0);
    pt1->packet = new Packet();
    unsigned char * message = pt1->toWireFormat();
    int size = pt1->getWireFormatSize(true);
    EXPECT_EQ(pt1->packet->getSize(), size);

    pt1->packet->resetForReading();
    ProduceTopic *pt2 = new ProduceTopic(pt1->packet);

    EXPECT_NE(pt2, (void*)0);
    EXPECT_EQ(pt2->packet->getSize(), pt1->packet->getSize());
    EXPECT_EQ(pt2->topicName, pt1->topicName);
    EXPECT_EQ(pt2->produceMessageSetArraySize, pt1->produceMessageSetArraySize);
    for (int i=0; i<pt2->produceMessageSetArraySize; i++) {
      EXPECT_EQ(*(pt2->produceMessageSetArray[i]), *(pt1->produceMessageSetArray[i]));
    }

    delete pt1->packet;
    delete pt1;
    delete pt2;
  }

}  // namespace
