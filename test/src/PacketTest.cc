#include <Packet.h>
#include <gtest/gtest.h>
#include "BaseTest.h"

namespace {

  class PacketTest : public BaseTest {
    protected:

      PacketTest() { }
      virtual ~PacketTest() { }
      virtual void SetUp() { } 
      virtual void TearDown() { }
  };

  TEST_F(PacketTest, Constructor) {
    Packet *p1 = new Packet();
    EXPECT_NE(p1, (void*)0);
    p1->beginCRC32();
    p1->writeInt32(1);
    p1->writeInt32(2);
    p1->writeInt32(3);
    p1->endCRC32();
    unsigned char * message = p1->toWireFormat();
    EXPECT_EQ(p1->getSize(), sizeof(int) + (4 * sizeof(int))); // size + crc + 3 int32s

    Packet *p2 = new Packet(message);
    EXPECT_NE(p2, (void*)0);
    EXPECT_EQ(p2->getSize(), p1->getSize());
    int crc = p2->readInt32();
    EXPECT_EQ(crc, -1889021706); // calculated *signed int* CRC32 of 3 byte sequence [1,2,3] from above

    delete p1;
    delete p2;
  }

}  // namespace
