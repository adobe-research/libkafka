#include <string>
#include <gtest/gtest.h>

using namespace std;

class BaseTest : public ::testing::Test {
  protected:

    BaseTest();
    virtual ~BaseTest();
    virtual void SetUp();
    virtual void TearDown();

    unsigned char packet[256];
    unsigned char *head;

    void init_packet();
    void write_int16(short int value);
    void write_int32(int value);
    void write_string(string value);
};
