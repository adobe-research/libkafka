#include <string>
#include <gtest/gtest.h>
#include "BaseTest.h"
#include <Packet.h>
#include <Message.h>
#include <metadata/PartitionMetadata.h>

namespace {

  class MessageTest : public BaseTest {
    protected:

      MessageTest() { }
      virtual ~MessageTest() { }
      virtual void SetUp() { } 
      virtual void TearDown() { }
  };

  TEST_F(MessageTest, Constructor) {
    Message *m1 = createMessage();
    EXPECT_NE(m1, (void*)0);
    m1->packet = new Packet();
    unsigned char * message = m1->toWireFormat();
    int size = m1->getWireFormatSize(true);
    EXPECT_EQ(m1->packet->getSize(), size);

    m1->packet->resetForReading();
    Message *m2 = new Message(m1->packet);

    EXPECT_NE(m2, (void*)0);
    EXPECT_EQ(m2->packet->getSize(), m1->packet->getSize());
    EXPECT_EQ(m2->crc, m1->crc);
    EXPECT_EQ(m2->magicByte, m1->magicByte);
    EXPECT_EQ(m2->attributes, m1->attributes);
    EXPECT_EQ(m2->keyLength, m1->keyLength);
    EXPECT_EQ(m2->valueLength, m1->valueLength);

    delete m1->packet;
    delete m1;
    delete m2;
  }

}  // namespace
