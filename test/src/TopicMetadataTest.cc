#include <string>
#include <gtest/gtest.h>
#include <BaseTest.h>
#include <Packet.h>
#include <TopicMetadata.h>
#include <PartitionMetadata.h>

namespace {

  class TopicMetadataTest : public BaseTest {
    protected:

      TopicMetadataTest() { }
      virtual ~TopicMetadataTest() { }
      virtual void SetUp() { } 
      virtual void TearDown() { }
  };

  TEST_F(TopicMetadataTest, Constructor) {
    TopicMetadata *tm1 = createTopicMetadata();
    EXPECT_NE(tm1, (void*)0);
    tm1->packet = new Packet();
    unsigned char * message = tm1->toWireFormat();
    int size = tm1->getWireFormatSize(true);
    EXPECT_EQ(tm1->packet->size, size);

    tm1->packet->resetForReading();
    TopicMetadata *tm2 = new TopicMetadata(tm1->packet);

    EXPECT_NE(tm2, (void*)0);
    EXPECT_EQ(tm2->packet->size, tm1->packet->size);
    EXPECT_EQ(tm2->topicErrorCode, tm1->topicErrorCode);
    EXPECT_EQ(tm2->topicName, tm1->topicName);
    EXPECT_EQ(tm2->partitionMetadataArraySize, tm1->partitionMetadataArraySize);
    for (int i=0; i<tm2->partitionMetadataArraySize; i++) {
      EXPECT_EQ(*(tm2->partitionMetadataArray[i]), *(tm1->partitionMetadataArray[i]));
    }
  }

}  // namespace
