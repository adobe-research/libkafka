#include <string>
#include <gtest/gtest.h>
#include <BaseTest.h>
#include <MetadataRequest.h>

namespace {

  class MetadataRequestTest : public BaseTest {
    protected:

      MetadataRequestTest() { }
      virtual ~MetadataRequestTest() { }
      virtual void SetUp() { } 
      virtual void TearDown() { }
  };

  TEST_F(MetadataRequestTest, Constructor) {
    short int apiKey = 1;
    short int apiVersion = 2;
    int correlationId = 3;
    string clientId = string("testClientId");
    int topicNameArraySize = 3;
    string topicNameArray[3] = { string("testTopic1"), string("testTopic2"), string("testTopic3") };

    MetadataRequest *mr1 = new MetadataRequest(apiKey, apiVersion, correlationId, clientId, topicNameArraySize, topicNameArray);
    EXPECT_NE(mr1, (void*)0);
    unsigned char * message = mr1->toWireFormat();
    int size = sizeof(int) + sizeof(short int) + sizeof(short int) + sizeof(int) + sizeof(short int) + clientId.length() + sizeof(int);
    for (int i=0; i<topicNameArraySize; i++) {
      size += sizeof(short int) + topicNameArray[i].length();
    }
    EXPECT_EQ(mr1->size(), size);

    MetadataRequest *mr2 = new MetadataRequest(message);

    EXPECT_NE(mr2, (void*)0);
    EXPECT_EQ(mr2->size(), mr1->size());
    EXPECT_EQ(mr2->apiKey, mr1->apiKey);
    EXPECT_EQ(mr2->apiVersion, mr1->apiVersion);
    EXPECT_EQ(mr2->correlationId, mr1->correlationId);
    EXPECT_EQ(mr2->clientId, mr1->clientId);
    EXPECT_EQ(mr2->topicNameArraySize, mr1->topicNameArraySize);
    for (int i=0; i<mr2->topicNameArraySize; i++) {
      EXPECT_EQ(mr2->topicNameArray[i], mr1->topicNameArray[i]);
    }
  }

}  // namespace
