///////////////////////////////////////////////////////////////////////////
//
// libkafka - C/C++ client for Apache Kafka v0.8+
//
// David Tompkins -- 8/8/2013
// tompkins@adobe_dot_com
//
///////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2013 Adobe Systems Incorporated. All rights reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
///////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <sstream>
#include <string>
#include <gtest/gtest.h>
#include "BaseTest.h"
#include "TestConfig.h"

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
    return new MetadataRequest(correlationId, clientId, 0, (string*)NULL);
  }
  
  topicNameArray = new string[topicNameArraySize];

  for (int i=0; i<topicNameArraySize; i++)
  {
    stringstream sstm;
    sstm << "topic" << i;
    topicNameArray[i] = sstm.str();
  }

  return new MetadataRequest(correlationId, clientId, topicNameArraySize, topicNameArray, true);
}

// Message

const char* BaseTest::defaultKey = "test_key";
unsigned char* BaseTest::key;
const char* BaseTest::defaultValue = "test_value";
unsigned char* BaseTest::value;

Message* BaseTest::createMessage()
{
  key = new unsigned char[keyLength];
  memcpy(key, defaultKey, keyLength);
  value = new unsigned char[valueLength];
  memcpy(value, defaultValue, valueLength);

  // crc + magicByte + attributes + key + value
  int messageSize = sizeof(int) + sizeof(signed char) + sizeof(signed char) + sizeof(int) + keyLength + sizeof(int) + valueLength;

  return new Message(crc, magicByte, attributes, keyLength, key, valueLength, value, offset, true);
}

// MessageSet

vector<Message*> BaseTest::messages;

MessageSet* BaseTest::createMessageSet()
{
  int messageSetSize = 0; 

  messages.clear();

  for (int i = 0 ; i < numMessages ; i++)
  {
    Message *message = createMessage();
    messages.push_back(message);
    // sizeof(offset) + sizeof(messageSize) + messageSize
    messageSetSize += sizeof(long int) + sizeof(int) + message->getWireFormatSize(false);
  }

  return new MessageSet(messageSetSize, messages, true);
}

// TopicNameBlock
FetchPartition** BaseTest::fetchPartitionArray;

TopicNameBlock<FetchPartition>* BaseTest::createTopicNameBlock(string topicName)
{
  fetchPartitionArray = new FetchPartition*[fetchPartitionArraySize];
  for (int i=0; i<fetchPartitionArraySize; i++) {
    fetchPartitionArray[i] = createFetchPartition();
  }
  return new TopicNameBlock<FetchPartition>(topicName, fetchPartitionArraySize, fetchPartitionArray, true);
}

// ProduceMessageSet

MessageSet* BaseTest::messageSet;

ProduceMessageSet* BaseTest::createProduceMessageSet()
{
  messageSet = createMessageSet();
  int messageSetSize = messageSet->getWireFormatSize(false);
  return new ProduceMessageSet(partition, messageSetSize, messageSet, true);
}

// TopicNameBlock for ProduceRequest

ProduceMessageSet** BaseTest::produceMessageSetArray;

TopicNameBlock<ProduceMessageSet>* BaseTest::createProduceRequestTopicNameBlock()
{
  produceMessageSetArray = new ProduceMessageSet*[produceMessageSetArraySize];
  for (int i=0; i<produceMessageSetArraySize; i++) {
    produceMessageSetArray[i] = createProduceMessageSet();
  }
  return new TopicNameBlock<ProduceMessageSet>(TestConfig::PRODUCE_REQUEST_TOPIC_NAME, produceMessageSetArraySize, produceMessageSetArray, true);
}

// ProduceRequest
TopicNameBlock<ProduceMessageSet>** BaseTest::produceTopicArray;

ProduceRequest* BaseTest::createProduceRequest()
{
  produceTopicArray = new TopicNameBlock<ProduceMessageSet>*[produceTopicArraySize];
  for (int i=0; i<produceTopicArraySize; i++) {
    produceTopicArray[i] = createProduceRequestTopicNameBlock();
  }
  return new ProduceRequest(correlationId, clientId, requiredAcks, timeout, produceTopicArraySize, produceTopicArray, true);
}

// ProduceResponsePartition

ProduceResponsePartition* BaseTest::createProduceResponsePartition()
{
  return new ProduceResponsePartition(partition, errorCode, offset);
}

// TopicNameBlock for ProduceResponse

ProduceResponsePartition** BaseTest::produceResponsePartitionArray;

TopicNameBlock<ProduceResponsePartition>* BaseTest::createProduceResponseTopicNameBlock()
{
  produceResponsePartitionArray = new ProduceResponsePartition*[produceResponsePartitionArraySize];
  for (int i=0; i<produceResponsePartitionArraySize; i++) {
    produceResponsePartitionArray[i] = createProduceResponsePartition();
  }
  return new TopicNameBlock<ProduceResponsePartition>(TestConfig::PRODUCE_RESPONSE_TOPIC_NAME, produceResponsePartitionArraySize, produceResponsePartitionArray, true);
}

// ProduceResponse
TopicNameBlock<ProduceResponsePartition>** BaseTest::produceResponseTopicArray;

ProduceResponse* BaseTest::createProduceResponse()
{
  produceResponseTopicArray = new TopicNameBlock<ProduceResponsePartition>*[produceResponseTopicArraySize];
  for (int i=0; i<produceResponseTopicArraySize; i++) {
    produceResponseTopicArray[i] = createProduceResponseTopicNameBlock();
  }
  return new ProduceResponse(correlationId, produceResponseTopicArraySize, produceResponseTopicArray, true);
}

// FetchRequest
TopicNameBlock<FetchPartition>** BaseTest::fetchTopicArray;

FetchRequest* BaseTest::createFetchRequest()
{
  fetchTopicArray = new TopicNameBlock<FetchPartition>*[fetchTopicArraySize];
  for (int i=0; i<fetchTopicArraySize; i++) {
    fetchTopicArray[i] = createTopicNameBlock(TestConfig::FETCH_RESPONSE_TOPIC_NAME);
  }
  return new FetchRequest(correlationId, clientId, replicaId, maxWaitTime, minBytes, fetchTopicArraySize, fetchTopicArray, true);
}

// FetchPartition
FetchPartition* BaseTest::createFetchPartition()
{
  return new FetchPartition(partition, fetchOffset, maxBytes);
}

// TopicNameBlock for FetchResponse

FetchResponsePartition** BaseTest::fetchResponsePartitionArray;

TopicNameBlock<FetchResponsePartition>* BaseTest::createFetchResponseTopicNameBlock()
{
  fetchResponsePartitionArray = new FetchResponsePartition*[fetchResponsePartitionArraySize];
  for (int i=0; i<fetchResponsePartitionArraySize; i++) {
    fetchResponsePartitionArray[i] = createFetchResponsePartition();
  }
  return new TopicNameBlock<FetchResponsePartition>(TestConfig::FETCH_RESPONSE_TOPIC_NAME, fetchResponsePartitionArraySize, fetchResponsePartitionArray, true);
}

// FetchResponsePartition

FetchResponsePartition* BaseTest::createFetchResponsePartition()
{
  messageSet = createMessageSet();
  int messageSetSize = messageSet->getWireFormatSize(false);
  return new FetchResponsePartition(partition, errorCode, highwaterMarkOffset, messageSetSize, messageSet, true);
}

// FetchResponse
TopicNameBlock<FetchResponsePartition>** BaseTest::fetchResponseTopicArray;

FetchResponse* BaseTest::createFetchResponse()
{
  fetchResponseTopicArray = new TopicNameBlock<FetchResponsePartition>*[fetchResponseTopicArraySize];
  for (int i=0; i<fetchResponseTopicArraySize; i++) {
    fetchResponseTopicArray[i] = createFetchResponseTopicNameBlock();
  }
  return new FetchResponse(correlationId, fetchResponseTopicArraySize, fetchResponseTopicArray, true);
}

// OffsetRequest
TopicNameBlock<OffsetPartition>** BaseTest::offsetTopicArray;

OffsetRequest* BaseTest::createOffsetRequest()
{
  offsetTopicArray = new TopicNameBlock<OffsetPartition>*[offsetTopicArraySize];
  for (int i=0; i<offsetTopicArraySize; i++) {
    offsetTopicArray[i] = createOffsetRequestTopicNameBlock(TestConfig::OFFSET_REQUEST_TOPIC_NAME);
  }
  return new OffsetRequest(correlationId, clientId, replicaId, offsetTopicArraySize, offsetTopicArray, true);
}

// TopicNameBlock for OffsetRequest

OffsetPartition** BaseTest::offsetPartitionArray;

TopicNameBlock<OffsetPartition>* BaseTest::createOffsetRequestTopicNameBlock(string topic_name)
{
  offsetPartitionArray = new OffsetPartition*[offsetPartitionArraySize];
  for (int i=0; i<offsetPartitionArraySize; i++) {
    offsetPartitionArray[i] = createOffsetPartition();
  }
  return new TopicNameBlock<OffsetPartition>(topic_name, offsetPartitionArraySize, offsetPartitionArray, true);
}

// OffsetPartition
OffsetPartition* BaseTest::createOffsetPartition()
{
  return new OffsetPartition(partition, time, maxNumberOfOffsets);
}

