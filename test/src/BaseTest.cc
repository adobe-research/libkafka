#include <iostream>
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

int BaseTest::getTopicMetadataPacketSize(string topicName, bool includePacketSize)
{
  // Packet.size 
  // topicErrorCode + topicName + partitionMetadataArraySize
  // partitionMetadataArraySize*PartitionMetadata

  int size = 0;
  if (includePacketSize) size += sizeof(int);
  size += sizeof(short int) + sizeof(short int) + topicName.length() + sizeof(int);
  for (int i=0; i<partitionMetadataArraySize; i++) {
    size += getPartitionMetadataPacketSize(false);
  }
  return size;
}

// PartitionMetadata

const int BaseTest::replicaArray[3] = { 1, 2, 3 };
const int BaseTest::isrArray[3] = { 4, 5, 6 };

PartitionMetadata* BaseTest::createPartitionMetadata(int partitionId)
{
  return new PartitionMetadata(partitionErrorCode, partitionId, leader, replicaArraySize, (int*)replicaArray, isrArraySize, (int*)isrArray);
}

int BaseTest::getPartitionMetadataPacketSize(bool includePacketSize)
{
  // Packet.size 
  // partitionErrorCode + partitionId + leader
  // replicaArraySize + replicaArraySize*int32
  // isrArraySize + isrArraySize*int32

  int size = 0;
  if (includePacketSize) size += sizeof(int);
  size += sizeof(short int) + sizeof(int) + sizeof(int);
  size += sizeof(int) + (replicaArraySize * sizeof(int));
  size += sizeof(int) + (isrArraySize * sizeof(int));
  return size;
}
