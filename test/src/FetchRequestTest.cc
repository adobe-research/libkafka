#include <string>
#include <gtest/gtest.h>
#include "BaseTest.h"

namespace {

  class FetchRequestTest : public BaseTest {
    protected:

      FetchRequestTest() { }
      virtual ~FetchRequestTest() { }
      virtual void SetUp() { } 
      virtual void TearDown() { }
  };

  TEST_F(FetchRequestTest, Constructor) {
    
    FetchRequest *fr1 = createFetchRequest();
    EXPECT_NE(fr1, (void*)0);
    unsigned char * message = fr1->toWireFormat();
    int size = fr1->getWireFormatSize(true);
    EXPECT_EQ(fr1->size(), size);

    FetchRequest *fr2 = new FetchRequest(message);

    EXPECT_NE(fr2, (void*)0);
    EXPECT_EQ(fr2->size(), fr1->size());
    EXPECT_EQ(fr2->replicaId, fr1->replicaId);
    EXPECT_EQ(fr2->maxWaitTime, fr1->maxWaitTime);
    EXPECT_EQ(fr2->minBytes, fr1->minBytes);
    for (int i=0; i<fr2->fetchTopicArraySize; i++) {
      EXPECT_EQ(*(fr2->fetchTopicArray[i]), *(fr1->fetchTopicArray[i]));
    }

    delete fr1;
    delete fr2;
  }

}  // namespace
