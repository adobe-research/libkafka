#include <string>
#include <gtest/gtest.h>

using namespace std;

#ifdef DEBUG 
#define D(x) x
#else 
#define D(x)
#endif

class BaseTest : public ::testing::Test {
  protected:

    BaseTest();
    virtual ~BaseTest();
    virtual void SetUp();
    virtual void TearDown();

    unsigned char packet[1024];
    unsigned char *head;

    void init_packet();
    void write_int16(short int value);
    void write_int32(int value);
    void write_string(string value);
};
