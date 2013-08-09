#include <string>
#include <gtest/gtest.h>
#include <BaseTest.h>

using namespace std;

BaseTest::BaseTest() { }
BaseTest::~BaseTest() { }
void BaseTest::SetUp() { } 
void BaseTest::TearDown() { }

void BaseTest::init_packet() { head = (unsigned char *)&packet; }

void BaseTest::write_int16(short int value)
{
  memcpy(head, &value, sizeof(short int));
  head += sizeof(short int);
}

void BaseTest::write_int32(int value)
{
  memcpy(head, &value, sizeof(int));
  head += sizeof(int);
}

void BaseTest::write_string(string value)
{
  short int length = value.length();
  memcpy(head, &length, sizeof(short int));
  head += sizeof(short int);
  memcpy(head, value.data(), length);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
