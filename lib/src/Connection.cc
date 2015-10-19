///////////////////////////////////////////////////////////////////////////
//
// libkafka - C/C++ client for Apache Kafka v0.8+
//
// David Tompkins -- 8/8/2013
// tompkins@adobe_dot_com
//
///////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2013 Adobe Systems Incorporated. All rights reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
///////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <string>
#include <cstring>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#include <errno.h>
#include <fcntl.h>

#include "Connection.h"
#include "Util.h"

using namespace std;

namespace LibKafka {

const int Connection::DEFAULT_BUFFER_SIZE;
const int Connection::SOCKET_UNINITIALIZED;
const int Connection::OPEN_CONNECTION_ERROR;
const int Connection::READ_ERROR;
const int Connection::WRITE_ERROR;
const int Connection::CONNECTION_RESET;

Connection::Connection(string host, int port)
{
  D(cout.flush() << "--------------Connection(incoming)\n";)

    this->host = host;
  this->port = port;
  this->portString = intToString(port);
  this->socketFd = SOCKET_UNINITIALIZED;
  this->host_info_list = NULL;
}

Connection::~Connection()
{
  D(cout.flush() << "--------------~Connection()\n";)

    this->close();
}

int Connection::open()
{
  D(cout.flush() << "--------------Connection::open()\n";)

    int status;

  memset(&(this->host_info), 0, sizeof(host_info));
  this->host_info.ai_family = AF_UNSPEC;
  this->host_info.ai_socktype = SOCK_STREAM;

  D(cout.flush() << "--------------Connection::open():getaddrinfo\n";)
    status = getaddrinfo(host.c_str(), portString.c_str(), &(this->host_info), &(this->host_info_list));
  if (status != 0)
  {
    E("Connect::open():getaddrinfo error:" << gai_strerror(status) << "\n");
    return OPEN_CONNECTION_ERROR;
  }

  D(cout.flush() << "--------------Connection::open():socket\n";)
  this->socketFd = socket(this->host_info_list->ai_family, this->host_info_list->ai_socktype, this->host_info_list->ai_protocol);
  if (socketFd == -1)
  {

    E("Connection::open():socket error:" << strerror(errno) << "\n");
    return OPEN_CONNECTION_ERROR;
  }

  struct timeval  timeout;
  timeout.tv_sec = 10;
  timeout.tv_usec = 0;
  fd_set set;
  FD_ZERO(&set);
  FD_SET(this->socketFd, &set);
  fcntl(this->socketFd, F_SETFL, O_NONBLOCK);

  D(cout.flush() << "--------------Connection::open():connect\n";)
  status = connect(socketFd, this->host_info_list->ai_addr, this->host_info_list->ai_addrlen);
  if ((status == -1) && (errno != EINPROGRESS))
  {
    E("Connection::open():open error:" << strerror(errno) << "\n");
    return OPEN_CONNECTION_ERROR;
  }

  status = select(this->socketFd+1, NULL, &set, NULL, &timeout);
  fcntl(this->socketFd, F_SETFL, fcntl(this->socketFd, F_GETFL, 0) & ~O_NONBLOCK);

  D(cout.flush() << "--------------Connection::open():connected\n";)
  return this->socketFd;
}

void Connection::close()
{
  D(cout.flush() << "--------------Connection::close()\n";)

  if (this->host_info_list != NULL)
  {
    freeaddrinfo(this->host_info_list);
    this->host_info_list = NULL;
  }

  if (this->socketFd != SOCKET_UNINITIALIZED)
  {
    ::close(this->socketFd);
    this->socketFd = SOCKET_UNINITIALIZED;
  }
}

int Connection::read(int numBytes, unsigned char* buffer)
{
  D(cout.flush() << "--------------Connection::read(" << numBytes << ")\n";)

  int flags = 0;
  int numBytesReceived = 0;
  unsigned char *p = buffer;

  while (numBytesReceived < numBytes)
  {
    int rcvd = (int)::recv(this->socketFd, p, (size_t)(numBytes-numBytesReceived), flags);
    if (rcvd == READ_ERROR) { E("Connection::read():error:" << strerror(errno) << "\n"); break; }
    if (rcvd == CONNECTION_RESET) { E("Connection::read():error:" << strerror(errno) << "\n"); numBytesReceived = READ_ERROR; break; }
    p += rcvd;
    numBytesReceived += rcvd;
    D(cout.flush() << "--------------Connection::read(" << numBytes << "):read " << rcvd << " bytes\n";)
  }
  
  D(cout.flush() << "--------------Connection::read(" << numBytes << "):" << numBytesReceived << " bytes received\n";)
  return numBytesReceived;
}

int Connection::write(int numBytes, unsigned char* buffer)
{
  D(cout.flush() << "--------------Connection::write(" << numBytes << ")\n";)

    int flags = 0;
  int numBytesSent = (int)::send(this->socketFd, (const void*)buffer, (ssize_t)numBytes, flags);
  if (numBytesSent == WRITE_ERROR) { E("Connection::write():error:" << strerror(errno) << "\n"); }
  D(cout.flush() << "--------------Connection::write(" << numBytes << "):wrote " << numBytesSent << "bytes\n";)
    return numBytesSent;
}

ostream& operator<< (ostream& os, const Connection& c)
{
  os << c.host << ":" << c.host << "\n";
  return os;
}

}; // namespace LibKafka
