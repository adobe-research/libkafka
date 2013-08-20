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
    int status = c->open();
    EXPECT_NE(status, Connection::OPEN_CONNECTION_ERROR);
    string request = "GET / HTTP/1.1\nhost: www.google.com\n\n";
    int length = request.length();
    int numBytesSent = c->write(length, (unsigned char*)(request.c_str()));
    EXPECT_EQ(numBytesSent, length);
    unsigned char* buffer = new unsigned char[1024];
    int numBytesReceived = c->read(1024, buffer);
    EXPECT_GT(numBytesReceived, 0);

    c->close();
    delete buffer;
    delete c;
  }

}  // namespace
