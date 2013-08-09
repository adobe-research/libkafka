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
};
