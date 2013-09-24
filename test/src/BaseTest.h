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

#include <string>
#include <vector>
#include <gtest/gtest.h>
#include <Debug.h>
#include <metadata/Broker.h>
#include <metadata/TopicMetadata.h>
#include <metadata/PartitionMetadata.h>
#include <metadata/MetadataRequest.h>
#include <metadata/MetadataResponse.h>
#include <Message.h>
#include <MessageSet.h>
#include <TopicNameBlock.h>
#include <produce/ProduceMessageSet.h>
#include <produce/ProduceRequest.h>
#include <produce/ProduceResponsePartition.h>
#include <produce/ProduceResponse.h>
#include <fetch/FetchPartition.h>
#include <fetch/FetchRequest.h>
#include <fetch/FetchResponse.h>
#include <fetch/FetchResponsePartition.h>
#include <offset/OffsetRequest.h>
#include <offset/OffsetPartition.h>
#include <offset/OffsetResponse.h>
#include <offset/OffsetResponsePartition.h>
#include "ApiConstants.h"

using namespace std;
using namespace LibKafka;

class BaseTest : public ::testing::Test {
  protected:

    BaseTest();
    virtual ~BaseTest();
    virtual void SetUp();
    virtual void TearDown();

    // Packet
    const static char *bytes;
    const static int numBytes;

    // Broker
    const static int nodeId = 2;
    const static int port = 8000;
    Broker *createBroker(string host = string("test_host"));

    // PartitionMetadata
    const static short int partitionErrorCode = 2;
    const static int leader = 2;
    const static int replicaArraySize = 3;
    const static int isrArraySize = 3;
    static int *replicaArray;
    static int *isrArray;
    PartitionMetadata *createPartitionMetadata(int partitionId = 9);

    // TopicMetadata
    const static short int topicErrorCode = 6;
    const static int partitionMetadataArraySize = 3;
    static PartitionMetadata **partitionMetadataArray;
    TopicMetadata *createTopicMetadata(string topicName = string("test"));

    // MetadataResponse
    const static int correlationId = 212121;
    const static int brokerArraySize = 3;
    const static int topicMetadataArraySize = 2;
    static Broker **brokerArray;
    static TopicMetadata **topicMetadataArray;
    MetadataResponse *createMetadataResponse();

    // MetadataRequest
    const static string clientId;
    const static int topicNameArraySize = 3;
    static string *topicNameArray;
    MetadataRequest *createMetadataRequest(bool emptyTopicArray = false);

    // MessageSet
    const static long int offset = 21;
    const static int numMessages = 3;
    static vector<Message*> messages;
    MessageSet *createMessageSet();

    // Message
    const static int crc = 1001;
    const static signed char magicByte = -1;
    const static signed char attributes = 0; // last three bits must be zero to disable gzip compression
    const static char *defaultKey; // "test_key", null terminated
    const static int keyLength = 9;
    static unsigned char *key;
    const static char *defaultValue; // "test_value", null terminated
    const static int valueLength = 11;
    static unsigned char *value;
    Message *createMessage();
    
    // ProduceMessageSet
    const static int partition = 0;
    static MessageSet *messageSet;
    ProduceMessageSet *createProduceMessageSet();

    // TopicNameBlock for ProduceRequest
    const static int produceMessageSetArraySize = 2;
    static ProduceMessageSet **produceMessageSetArray;
    TopicNameBlock<ProduceMessageSet> *createProduceRequestTopicNameBlock();

    // ProduceRequest
    const static int requiredAcks = 1;
    const static int timeout = 20;
    const static int produceTopicArraySize = 1;
    static TopicNameBlock<ProduceMessageSet> **produceTopicArray;
    ProduceRequest *createProduceRequest();

    // ProduceResponsePartition
    const static short int errorCode = 0;
    ProduceResponsePartition *createProduceResponsePartition();

    // TopicNameBlock for ProduceResponse
    const static int produceResponsePartitionArraySize = 1;
    static ProduceResponsePartition** produceResponsePartitionArray;
    TopicNameBlock<ProduceResponsePartition>* createProduceResponseTopicNameBlock();
    
    // ProduceResponse
    const static int produceResponseTopicArraySize = 3;
    static TopicNameBlock<ProduceResponsePartition> **produceResponseTopicArray;
    ProduceResponse *createProduceResponse();

    // FetchPartition
    const static long int fetchOffset = 100;
    const static int maxBytes = 4096;
    FetchPartition *createFetchPartition(long offset = fetchOffset);
    
    // TopicNameBlock
    const static int fetchPartitionArraySize = 1;
    static FetchPartition **fetchPartitionArray;
    TopicNameBlock<FetchPartition> *createTopicNameBlock(string topicName = string("test"), long offset = fetchOffset);
    
    // FetchRequest
    const static int replicaId = -1;
    const static int maxWaitTime = 100;
    const static int minBytes = 0;
    const static int fetchTopicArraySize = 1;
    static TopicNameBlock<FetchPartition> **fetchTopicArray;
    FetchRequest *createFetchRequest(long offset = fetchOffset);

    // TopicNameBlock for FetchResponse
    const static int fetchResponsePartitionArraySize = 1;
    static FetchResponsePartition** fetchResponsePartitionArray;
    TopicNameBlock<FetchResponsePartition>* createFetchResponseTopicNameBlock();
    
    // FetchResponse
    const static int fetchResponseTopicArraySize = 3;
    static TopicNameBlock<FetchResponsePartition> **fetchResponseTopicArray;
    FetchResponse *createFetchResponse();
    
    // FetchResponsePartition
    const static long int highwaterMarkOffset = 21;
    FetchResponsePartition *createFetchResponsePartition();
    
    // OffsetRequest
    const static int offsetTopicArraySize = 1;
    static TopicNameBlock<OffsetPartition> **offsetTopicArray;
    OffsetRequest *createOffsetRequest();
    
    // TopicNameBlock for OffsetRequest
    const static int offsetPartitionArraySize = 2;
    static OffsetPartition **offsetPartitionArray;
    TopicNameBlock<OffsetPartition> *createOffsetRequestTopicNameBlock(std::string topic_name);

    // OffsetPartition
    //const static long int time = -2; // earliest available
    const static long int time = -1; // latest available
    const static int maxNumberOfOffsets = 16;
    OffsetPartition *createOffsetPartition();
    
    // TopicNameBlock for OffsetResponse
    const static int offsetResponsePartitionArraySize = 1;
    static OffsetResponsePartition** offsetResponsePartitionArray;
    TopicNameBlock<OffsetResponsePartition>* createOffsetResponseTopicNameBlock();
    
    // OffsetResponse
    const static int offsetResponseTopicArraySize = 3;
    static TopicNameBlock<OffsetResponsePartition> **offsetResponseTopicArray;
    OffsetResponse *createOffsetResponse();
    
    // OffsetResponsePartition
    const static int offsetArraySize = 3;
    static long int *offsetArray;
    OffsetResponsePartition *createOffsetResponsePartition();
};
