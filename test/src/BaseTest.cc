#include <iostream>
#include <sstream>
#include <string>
#include <gtest/gtest.h>
#include <BaseTest.h>

using namespace std;

BaseTest::BaseTest() { }
BaseTest::~BaseTest() { }
void BaseTest::SetUp() { } 
void BaseTest::TearDown() { }

int main(int argc, char **argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

// Broker

Broker* BaseTest::createBroker(string host)
{
  return new Broker(nodeId, host, port);
}

// TopicMetadata

const PartitionMetadata* BaseTest::partitionMetadataArray[3];

TopicMetadata* BaseTest::createTopicMetadata(string topicName)
{
  for (int i=0; i<partitionMetadataArraySize; i++) {
    int partitionId = i;
    partitionMetadataArray[i] = createPartitionMetadata(partitionId);
  }

  return new TopicMetadata(topicErrorCode, topicName, partitionMetadataArraySize, (PartitionMetadata**)partitionMetadataArray);
}

// PartitionMetadata

const int BaseTest::replicaArray[3] = { 1, 2, 3 };
const int BaseTest::isrArray[3] = { 4, 5, 6 };

PartitionMetadata* BaseTest::createPartitionMetadata(int partitionId)
{
  return new PartitionMetadata(partitionErrorCode, partitionId, leader, replicaArraySize, (int*)replicaArray, isrArraySize, (int*)isrArray);
}

// MetadataResponse

const Broker* BaseTest::brokerArray[3];
const TopicMetadata* BaseTest::topicMetadataArray[3];

MetadataResponse* BaseTest::createMetadataResponse()
{
  for (int i=0; i<brokerArraySize; i++) {
    stringstream sstm;
    sstm << "host" << i;
    brokerArray[i] = createBroker(sstm.str());
  }
  
  for (int i=0; i<topicMetadataArraySize; i++) {
    stringstream sstm;
    sstm << "topic" << i;
    topicMetadataArray[i] = createTopicMetadata(sstm.str());
  }

  return new MetadataResponse(correlationId, brokerArraySize, (Broker**)brokerArray, topicMetadataArraySize, (TopicMetadata**)topicMetadataArray);
}

// MetadataRequest

const string BaseTest::topicNameArray[3] = { string("testTopic1"), string("testTopic2"), string("testTopic3") };
const string BaseTest::clientId = string("libkafka-test");

MetadataRequest* BaseTest::createMetadataRequest(bool emptyTopicArray)
{
  if (emptyTopicArray)
  {
    return new MetadataRequest(apiVersion, correlationId, clientId, 0, (string*)NULL);
  }
  
  return new MetadataRequest(apiVersion, correlationId, clientId, topicNameArraySize, (string*)topicNameArray);
}
