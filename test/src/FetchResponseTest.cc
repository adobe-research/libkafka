#include <string>
#include <gtest/gtest.h>
#include "BaseTest.h"

namespace {

  class FetchResponseTest : public BaseTest {
    protected:

      FetchResponseTest() { }
      virtual ~FetchResponseTest() { }
      virtual void SetUp() { } 
      virtual void TearDown() { }
  };

  TEST_F(FetchResponseTest, Constructor) {
    
    FetchResponse *fr1 = createFetchResponse();
    EXPECT_NE(fr1, (void*)0);
    unsigned char * message = fr1->toWireFormat();
    int size = fr1->getWireFormatSize(true);
    EXPECT_EQ(fr1->size(), size);

    FetchResponse *fr2 = new FetchResponse(message); 
    EXPECT_NE(fr2, (void*)0);
    EXPECT_EQ(fr2->size(), fr1->size());
    EXPECT_EQ(fr2->fetchResponseTopicArraySize, fr1->fetchResponseTopicArraySize);
    for (int i=0; i<fr2->fetchResponseTopicArraySize; i++) {
      EXPECT_EQ(*(fr2->fetchResponseTopicArray[i]), *(fr1->fetchResponseTopicArray[i]));
    }

    delete fr1;
    delete fr2;
  }

}  // namespace
