#include <string>
#include <gtest/gtest.h>
#include <Debug.h>
#include <Broker.h>
#include <TopicMetadata.h>
#include <PartitionMetadata.h>
#include <MetadataRequest.h>
#include <MetadataResponse.h>
#include <ApiConstants.h>

using namespace std;
using namespace LibKafka;

class BaseTest : public ::testing::Test {
  protected:

    BaseTest();
    virtual ~BaseTest();
    virtual void SetUp();
    virtual void TearDown();

    // Broker
    const static int nodeId = 2;
    const static int port = 8000;
    Broker *createBroker(string host = string("test_host"));

    // PartitionMetadata
    const static short int partitionErrorCode = 2;
    const static int leader = 2;
    const static int replicaArraySize = 3;
    const static int replicaArray[];
    const static int isrArraySize = 3;
    const static int isrArray[];
    PartitionMetadata *createPartitionMetadata(int partitionId = 9);

    // TopicMetadata
    const static short int topicErrorCode = 6;
    const static int partitionMetadataArraySize = 3;
    const static PartitionMetadata *partitionMetadataArray[];
    TopicMetadata *createTopicMetadata(string topicName = string("test_topic"));

    // MetadataResponse
    const static int correlationId = 212121;
    const static int brokerArraySize = 3;
    const static Broker *brokerArray[];
    const static int topicMetadataArraySize = 2;
    const static TopicMetadata *topicMetadataArray[];
    MetadataResponse *createMetadataResponse();

    //MetadataRequest
    const static short int apiKey = ApiConstants::METADATA_REQUEST_KEY;
    const static short int apiVersion = ApiConstants::API_VERSION;
    const static string clientId;
    const static int topicNameArraySize = 3;
    const static string topicNameArray[];
    MetadataRequest *createMetadataRequest(bool emptyTopicArray = false);
};
