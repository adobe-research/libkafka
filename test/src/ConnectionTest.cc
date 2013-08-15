#include <string>
#include <gtest/gtest.h>
#include <BaseTest.h>
#include <Connection.h>

namespace {

  class ConnectionTest : public BaseTest {
    protected:

      ConnectionTest() { }
      virtual ~ConnectionTest() { }
      virtual void SetUp() { } 
      virtual void TearDown() { }
  };

  TEST_F(ConnectionTest, Constructor) {

    Connection *c = new Connection("www.google.com", 80);
    EXPECT_NE(c, (void*)0);
    int status = c->openConnection();
    EXPECT_NE(status, Connection::OPEN_CONNECTION_ERROR);
    c->closeConnection();
  }

}  // namespace
