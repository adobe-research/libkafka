#include <string>
#include <gtest/gtest.h>
#include <BaseTest.h>
#include <MetadataRequest.h>

namespace {

  class MetadataTest : public BaseTest {
    protected:

      MetadataTest() { }
      virtual ~MetadataTest() { }
      virtual void SetUp() { } 
      virtual void TearDown() { }
  };

  TEST_F(MetadataTest, Constructor) {
    init_packet();

    int size = 21;
    short int apiKey = 1;
    short int apiVersion = 2;
    int correlationId = 3;
    string clientId = string("testClientId");
    int topicNameArraySize = 3;
    string topicNameArray[3] = { string("testTopic1"), string("testTopic2"), string("testTopic3") };

    write_int32(size);
    write_int16(apiKey);
    write_int16(apiVersion);
    write_int32(correlationId);
    write_string(clientId);
    write_int32(topicNameArraySize);
    for (int i=0; i<topicNameArraySize; i++) {
      write_string(topicNameArray[i]);
    }

    MetadataRequest *r = new MetadataRequest((unsigned char *)&packet);

    EXPECT_NE(r, (void*)0);
    EXPECT_EQ(r->size, size);
    EXPECT_EQ(r->apiKey, apiKey);
    EXPECT_EQ(r->apiVersion, apiVersion);
    EXPECT_EQ(r->correlationId, correlationId);
    EXPECT_EQ(r->clientId, clientId);
    EXPECT_EQ(r->topicNameArraySize, topicNameArraySize);
    for (int i=0; i<r->topicNameArraySize; i++) {
      EXPECT_EQ(r->topicNameArray[i], topicNameArray[i]);
    }
  }

}  // namespace
