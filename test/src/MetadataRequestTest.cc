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

    MetadataRequest *mr1 = createMetadataRequest();
    EXPECT_NE(mr1, (void*)0);
    unsigned char * message = mr1->toWireFormat();
    int size = mr1->getWireFormatSize(true);
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
