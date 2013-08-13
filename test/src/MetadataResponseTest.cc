#include <string>
#include <gtest/gtest.h>
#include <BaseTest.h>
#include <MetadataResponse.h>
#include <Broker.h>

namespace {

  class MetadataResponseTest : public BaseTest {
    protected:

      MetadataResponseTest() { }
      virtual ~MetadataResponseTest() { }
      virtual void SetUp() { } 
      virtual void TearDown() { }
  };

  TEST_F(MetadataResponseTest, Constructor) {
    int correlationId = 3;
    int brokerArraySize = 3;
    Broker *brokerArray[brokerArraySize];
    for (int i=0; i<brokerArraySize; i++) {
      stringstream sstm;
      sstm << "host" << i;
      string host = sstm.str();
      brokerArray[i] = new Broker(i, host, 8000+i);
    }

    int topicMetadataArraySize = 2;
    TopicMetadata *topicMetadataArray[topicMetadataArraySize];
    short int topicErrorCode = 3;
    string topicName = string("test_topic");
    int partitionMetadataArraySize = 3;
    short int partitionErrorCode = 2;
    int leader = 21;
    int replicaArraySize = 3;
    int replicaArray[] = { 1, 2, 3 };
    int isrArraySize = 3;
    int isrArray[] = { 4, 5, 6 };
    PartitionMetadata *partitionMetadataArray[partitionMetadataArraySize];
    for (int i=0; i<topicMetadataArraySize; i++) {
      for (int ii=0; ii<partitionMetadataArraySize; ii++) {
	int partitionId = ii;
	partitionMetadataArray[ii] = new PartitionMetadata(partitionErrorCode, partitionId, leader, replicaArraySize, replicaArray, isrArraySize, isrArray);
      }
      topicMetadataArray[i] = new TopicMetadata(topicErrorCode, topicName, partitionMetadataArraySize, partitionMetadataArray);
    }

    MetadataResponse *mr1 = new MetadataResponse(correlationId, brokerArraySize, brokerArray, topicMetadataArraySize, topicMetadataArray);
    EXPECT_NE(mr1, (void*)0);
    unsigned char * message = mr1->toWireFormat();
    int size = sizeof(int) + sizeof(int) + sizeof(int);
    for (int i=0; i<brokerArraySize; i++) {
      size += sizeof(int) + sizeof(int);
      size += sizeof(short int) + brokerArray[i]->host.length();
    }
    size += sizeof(int);
    for (int i=0; i<topicMetadataArraySize; i++) {
      size += sizeof(short int) + sizeof(short int) + topicName.length() + sizeof(int);
      for (int i=0; i<partitionMetadataArraySize; i++) {
	size += sizeof(short int) + sizeof(int) + sizeof(int);
	size += sizeof(int) + (partitionMetadataArray[i]->replicaArraySize * sizeof(int));
	size += sizeof(int) + (partitionMetadataArray[i]->isrArraySize * sizeof(int));
      }
    }
    EXPECT_EQ(mr1->size(), size);

    MetadataResponse *mr2 = new MetadataResponse(message);

    EXPECT_NE(mr2, (void*)0);
    EXPECT_EQ(mr2->size(), mr1->size());
    EXPECT_EQ(mr2->correlationId, mr1->correlationId);
    EXPECT_EQ(mr2->brokerArraySize, mr1->brokerArraySize);
    for (int i=0; i<mr2->brokerArraySize; i++) {
      EXPECT_EQ(*(mr2->brokerArray[i]), *(mr1->brokerArray[i]));
    }
    for (int i=0; i<mr2->topicMetadataArraySize; i++) {
      EXPECT_EQ(*(mr2->topicMetadataArray[i]), *(mr1->topicMetadataArray[i]));
    }
  }

}  // namespace
