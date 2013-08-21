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

PartitionMetadata** BaseTest::partitionMetadataArray;

TopicMetadata* BaseTest::createTopicMetadata(string topicName)
{
  partitionMetadataArray = new PartitionMetadata*[partitionMetadataArraySize];

  for (int i=0; i<partitionMetadataArraySize; i++) {
    int partitionId = i;
    partitionMetadataArray[i] = createPartitionMetadata(partitionId);
  }

  return new TopicMetadata(topicErrorCode, topicName, partitionMetadataArraySize, partitionMetadataArray, true);
}

// PartitionMetadata

int* BaseTest::replicaArray;
int* BaseTest::isrArray;

PartitionMetadata* BaseTest::createPartitionMetadata(int partitionId)
{
  replicaArray = new int[replicaArraySize];
  isrArray = new int[isrArraySize];

  for (int i=0; i<replicaArraySize; i++) { replicaArray[i] = i; }
  for (int i=0; i<isrArraySize; i++) { isrArray[i] = i; }

  return new PartitionMetadata(partitionErrorCode, partitionId, leader, replicaArraySize, replicaArray, isrArraySize, isrArray, true);
}

// MetadataResponse

Broker** BaseTest::brokerArray;
TopicMetadata** BaseTest::topicMetadataArray;

MetadataResponse* BaseTest::createMetadataResponse()
{
  brokerArray = new Broker*[brokerArraySize];
  topicMetadataArray = new TopicMetadata*[topicNameArraySize];

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

  return new MetadataResponse(correlationId, brokerArraySize, brokerArray, topicMetadataArraySize, topicMetadataArray, true);
}

// MetadataRequest

string* BaseTest::topicNameArray;
const string BaseTest::clientId = string("libkafka-test");

MetadataRequest* BaseTest::createMetadataRequest(bool emptyTopicArray)
{
  if (emptyTopicArray)
  {
    return new MetadataRequest(apiVersion, correlationId, clientId, 0, (string*)NULL);
  }
  
  topicNameArray = new string[topicNameArraySize];

  for (int i=0; i<topicNameArraySize; i++)
  {
    stringstream sstm;
    sstm << "topic" << i;
    topicNameArray[i] = sstm.str();
  }

  return new MetadataRequest(apiVersion, correlationId, clientId, topicNameArraySize, topicNameArray, true);
}

// MessageSet

unsigned char* BaseTest::key;
unsigned char* BaseTest::value;

MessageSet* BaseTest::createMessageSet()
{
  key = new unsigned char[keyLength];
  value = new unsigned char[valueLength];

  // crc + magicByte + attributes + key + value
  int messageSize = sizeof(int) + sizeof(signed char) + sizeof(signed char) + sizeof(int) + keyLength + sizeof(int) + valueLength;

  return new MessageSet(offset, messageSize, crc, magicByte, attributes, keyLength, key, valueLength, value, true);
}

// ProduceMessageSet

MessageSet* BaseTest::messageSet;

ProduceMessageSet* BaseTest::createProduceMessageSet()
{
  messageSet = createMessageSet();
  int messageSetSize = messageSet->getWireFormatSize(false);
  return new ProduceMessageSet(partition, messageSetSize, messageSet, true);
}

// ProduceTopic

ProduceMessageSet** BaseTest::produceMessageSetArray;
const string BaseTest::topicName = string("test_topic");

ProduceTopic* BaseTest::createProduceTopic()
{
  produceMessageSetArray = new ProduceMessageSet*[produceMessageSetArraySize];
  for (int i=0; i<produceMessageSetArraySize; i++) {
    produceMessageSetArray[i] = createProduceMessageSet();
  }
  return new ProduceTopic(topicName, produceMessageSetArraySize, produceMessageSetArray, true);
}

// ProduceRequest
ProduceTopic** BaseTest::produceTopicArray;

ProduceRequest* BaseTest::createProduceRequest()
{
  produceTopicArray = new ProduceTopic*[produceTopicArraySize];
  for (int i=0; i<produceTopicArraySize; i++) {
    produceTopicArray[i] = createProduceTopic();
  }
  return new ProduceRequest(apiVersion, correlationId, clientId, requiredAcks, timeout, produceTopicArraySize, produceTopicArray, true);
}
