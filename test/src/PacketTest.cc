#include <Packet.h>
#include <gtest/gtest.h>
#include <BaseTest.h>

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
    unsigned char * message = p1->toWireFormat();
    EXPECT_EQ(p1->size, sizeof(int));

    Packet *p2 = new Packet(message);
    EXPECT_NE(p2, (void*)0);
    EXPECT_EQ(p2->size, p1->size);
  }

}  // namespace
