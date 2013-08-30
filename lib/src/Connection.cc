//////////////////////////////////////////////////////////////////
//                                                              //
// libkafka - C/C++ client for Apache Kafka v0.8+               //
//                                                              //
// David Tompkins -- 8/8/2013                                   //
// http://dt.org/                                               //
//                                                              //
// Copyright (c) 2013 by David Tompkins.                        //
//                                                              //
//////////////////////////////////////////////////////////////////
//                                                              //
// This program is free software; you can redistribute it       //
// and/or modify it under the terms of the GNU General Public   //
// License as published by the Free Software Foundation.        //
//                                                              //
// This program is distributed in the hope that it will be      //
// useful, but WITHOUT ANY WARRANTY; without even the implied   //
// warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR      //
// PURPOSE. See the GNU General Public License for more details //
//                                                              //
// You should have received a copy of the GNU General Public    //
// License along with this program; if not, write to the Free   //
// Software Foundation, Inc., 59 Temple Place, Suite 330,       //
// Boston, MA 02111-1307 USA                                    //
//                                                              //
//////////////////////////////////////////////////////////////////

#include <iostream>
#include <string>
#include <cstring>
#include <sys/socket.h>
#include <netdb.h>
#include <errno.h>

#include "Connection.h"
#include "Util.h"

using namespace std;

namespace LibKafka {

const int Connection::DEFAULT_BUFFER_SIZE;
const int Connection::SOCKET_UNINITIALIZED;
const int Connection::OPEN_CONNECTION_ERROR;
const int Connection::READ_ERROR;
const int Connection::WRITE_ERROR;

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

  D(cout.flush() << "--------------Connection::open():connect\n";)
    status = connect(socketFd, this->host_info_list->ai_addr, this->host_info_list->ai_addrlen);
  if (status == -1)
  {
    E("Connection::open():open error:" << strerror(errno) << "\n");
    return OPEN_CONNECTION_ERROR;
  }

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
  int numBytesReceived = (int)::recv(this->socketFd, buffer, (size_t)numBytes, flags);
  if (numBytesReceived == READ_ERROR) { E("Connection::read():error:" << strerror(errno) << "\n"); }
  D(cout.flush() << "--------------Connection::read(" << numBytes << "):read " << numBytesReceived << "bytes\n";)
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
