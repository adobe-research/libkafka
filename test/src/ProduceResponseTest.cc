#include <string>
#include <gtest/gtest.h>
#include "BaseTest.h"

namespace {

  class ProduceResponseTest : public BaseTest {
    protected:

      ProduceResponseTest() { }
      virtual ~ProduceResponseTest() { }
      virtual void SetUp() { } 
      virtual void TearDown() { }
  };

  TEST_F(ProduceResponseTest, Constructor) {
    
    ProduceResponse *pr1 = createProduceResponse();
    EXPECT_NE(pr1, (void*)0);
    unsigned char * message = pr1->toWireFormat();
    int size = pr1->getWireFormatSize(true);
    EXPECT_EQ(pr1->size(), size);

    ProduceResponse *pr2 = new ProduceResponse(message);

    EXPECT_NE(pr2, (void*)0);
    EXPECT_EQ(pr2->size(), pr1->size());
    EXPECT_EQ(pr2->produceResponseTopicArraySize, pr1->produceResponseTopicArraySize);
    for (int i=0; i<pr2->produceResponseTopicArraySize; i++) {
      EXPECT_EQ(*(pr2->produceResponseTopicArray[i]), *(pr1->produceResponseTopicArray[i]));
    }

    delete pr1;
    delete pr2;
  }

}  // namespace
