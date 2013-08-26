#include <string>
#include <gtest/gtest.h>
#include "BaseTest.h"

namespace {

  class ProduceRequestTest : public BaseTest {
    protected:

      ProduceRequestTest() { }
      virtual ~ProduceRequestTest() { }
      virtual void SetUp() { } 
      virtual void TearDown() { }
  };

  TEST_F(ProduceRequestTest, Constructor) {
    
    ProduceRequest *pr1 = createProduceRequest();
    EXPECT_NE(pr1, (void*)0);
    unsigned char * message = pr1->toWireFormat();
    int size = pr1->getWireFormatSize(true);
    EXPECT_EQ(pr1->size(), size);

    ProduceRequest *pr2 = new ProduceRequest(message);

    EXPECT_NE(pr2, (void*)0);
    EXPECT_EQ(pr2->size(), pr1->size());
    EXPECT_EQ(pr2->requiredAcks, pr1->requiredAcks);
    EXPECT_EQ(pr2->timeout, pr1->timeout);
    for (int i=0; i<pr2->produceTopicArraySize; i++) {
      EXPECT_EQ(*(pr2->produceTopicArray[i]), *(pr1->produceTopicArray[i]));
    }

    delete pr1;
    delete pr2;
  }

}  // namespace
