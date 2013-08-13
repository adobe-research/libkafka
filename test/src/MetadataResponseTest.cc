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
    int correlationId = 3;

    int brokerArraySize = 3;
    Broker *brokerArray[brokerArraySize];
    for (int i=0; i<brokerArraySize; i++) {
      stringstream sstm;
      sstm << "host" << i;
      string host = sstm.str();
      brokerArray[i] = new Broker(i, host, 8000+i);
    }

    int topicMetadataArraySize = 0;
    TopicMetadata **topicMetadataArray = NULL;
    //TopicMetadata *topicMetadataArray[topicMetadataArraySize];

    MetadataResponse *mr1 = new MetadataResponse(correlationId, brokerArraySize, brokerArray, topicMetadataArraySize, topicMetadataArray);
    EXPECT_NE(mr1, (void*)0);
    unsigned char * message = mr1->toWireFormat();
    int size = sizeof(int) + sizeof(int) + sizeof(int);
    for (int i=0; i<brokerArraySize; i++) {
      size += sizeof(int) + sizeof(int);
      size += sizeof(short int) + brokerArray[i]->host.length();
    }
    for (int i=0; i<topicMetadataArraySize; i++) {
    }
    EXPECT_EQ(mr1->size(), size);

    MetadataResponse *mr2 = new MetadataResponse(message);

    EXPECT_NE(mr2, (void*)0);
    EXPECT_EQ(mr2->size(), mr1->size());
    EXPECT_EQ(mr2->correlationId, mr1->correlationId);
    EXPECT_EQ(mr2->brokerArraySize, mr1->brokerArraySize);
    for (int i=0; i<mr2->brokerArraySize; i++) {
      EXPECT_EQ(*(mr2->brokerArray[i]), *(mr1->brokerArray[i]));
    }
    // add topicMetadataArray
  }

}  // namespace
