#include <string>
#include <gtest/gtest.h>
#include <Debug.h>
#include <TopicMetadata.h>
#include <PartitionMetadata.h>

using namespace std;

class BaseTest : public ::testing::Test {
  protected:

    BaseTest();
    virtual ~BaseTest();
    virtual void SetUp();
    virtual void TearDown();

    // TopicMetadata
    const static short int topicErrorCode = 6;
    const static int partitionMetadataArraySize = 3;
    const static PartitionMetadata *partitionMetadataArray[];
    TopicMetadata *createTopicMetadata(string topicName = string("test_topic"));
    int getTopicMetadataPacketSize(string topicName, bool includePacketSize = true);

    // PartitionMetadata
    const static short int partitionErrorCode = 2;
    const static int leader = 21;
    const static int replicaArraySize = 3;
    const static int replicaArray[];
    const static int isrArraySize = 3;
    const static int isrArray[];
    PartitionMetadata *createPartitionMetadata(int partitionId = 9);
    int getPartitionMetadataPacketSize(bool includePacketSize = true);
};
