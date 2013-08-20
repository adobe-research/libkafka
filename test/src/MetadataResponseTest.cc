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
    
    MetadataResponse *mr1 = createMetadataResponse();
    EXPECT_NE(mr1, (void*)0);
    unsigned char * message = mr1->toWireFormat();
    int size = mr1->getWireFormatSize(true);
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

    delete mr1;
    delete mr2;
  }

}  // namespace
