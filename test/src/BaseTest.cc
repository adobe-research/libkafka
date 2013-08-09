#include <iostream>
#include <string>
#include <gtest/gtest.h>
#include <BaseTest.h>

using namespace std;

BaseTest::BaseTest() { }
BaseTest::~BaseTest() { }
void BaseTest::SetUp() { } 
void BaseTest::TearDown() { }

void BaseTest::init_packet()
{
  head = (unsigned char *)&packet;
  D(cout << "init_packet()\n";)
}

void BaseTest::write_int16(short int value)
{
  memcpy(head, &value, sizeof(short int));
  head += sizeof(short int);
  D(cout << "write_int16():" << value << "\n";)
}

void BaseTest::write_int32(int value)
{
  memcpy(head, &value, sizeof(int));
  head += sizeof(int);
  D(cout << "write_int32():" << value << "\n";)
}

void BaseTest::write_int64(long int value)
{
  memcpy(head, &value, sizeof(long int));
  head += sizeof(long int);
  D(cout << "write_int64():" << value << "\n";)
}

void BaseTest::write_string(string value)
{
  short int length = value.length();
  memcpy(head, &length, sizeof(short int));
  head += sizeof(short int);
  memcpy(head, value.c_str(), length);
  head += length;
  D(cout << "write_string():" << length << ":" << value.c_str() << "\n";)
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
