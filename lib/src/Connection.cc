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
#include <sys/socket.h>
#include <netdb.h>

#include <Connection.h>
#include <Util.h>

using namespace std;

const int Connection::SOCKET_UNINITIALIZED;
const int Connection::OPEN_CONNECTION_ERROR;

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

  this->closeConnection();
}

int Connection::openConnection()
{
  D(cout.flush() << "--------------~Connection::openConnection()\n";)

  int status;

  memset(&(this->host_info), 0, sizeof(host_info));
  this->host_info.ai_family = AF_UNSPEC;
  this->host_info.ai_socktype = SOCK_STREAM;

  D(cout.flush() << "--------------~Connection::openConnection():getaddrinfo\n";)
  status = getaddrinfo(host.c_str(), portString.c_str(), &(this->host_info), &(this->host_info_list));
  if (status != 0)
  {
    E("Connect::openConnection():getaddrinfo error" << gai_strerror(status) << "\n");
    return OPEN_CONNECTION_ERROR;
  }

  D(cout.flush() << "--------------~Connection::openConnection():socket\n";)
  this->socketFd = socket(this->host_info_list->ai_family, this->host_info_list->ai_socktype, this->host_info_list->ai_protocol);
  if (socketFd == -1)
  {
    E("Connection::openConnection():socket error\n");
    return OPEN_CONNECTION_ERROR;
  }

  D(cout.flush() << "--------------~Connection::openConnection():connect\n";)
  status = connect(socketFd, this->host_info_list->ai_addr, this->host_info_list->ai_addrlen);
  if (status == -1)
  {
    E("Connection::openConnection():open error\n");
    return OPEN_CONNECTION_ERROR;
  }

  D(cout.flush() << "--------------~Connection::openConnection():connected\n";)
  return this->socketFd;
}

void Connection::closeConnection()
{
  D(cout.flush() << "--------------~Connection::closeConnection()\n";)

  if (this->host_info_list != NULL) freeaddrinfo(this->host_info_list);
  if (this->socketFd != SOCKET_UNINITIALIZED) close(this->socketFd);
}

ostream& operator<< (ostream& os, const Connection& c)
{
  os << c.host << ":" << c.host << "\n";
  return os;
}
