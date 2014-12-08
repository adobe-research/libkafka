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

#ifndef CONNECTION_H
#define CONNECTION_H

#include <iostream>
#include <string>
#if defined(WIN32)
	#include <Winsock2.h>
#else
	#include <sys/socket.h>
	#include <netdb.h>
#endif
#include "Debug.h"

namespace LibKafka {

class Connection
{
  public:

    static const int DEFAULT_BUFFER_SIZE = 1024;
    static const int SOCKET_UNINITIALIZED = -1;
    static const int OPEN_CONNECTION_ERROR = -1;
    static const int READ_ERROR = -1;
    static const int WRITE_ERROR = -1;

    Connection(std::string host, int port);
    ~Connection();
    
    int open();
    void close();
    int read(int numBytes, unsigned char *buffer);
    int write(int numBytes, unsigned char *buffer);

    std::string host;
    int port;

  protected:

    int socketFd;
    std::string portString;
    struct addrinfo host_info;
    struct addrinfo *host_info_list;
};

std::ostream& operator<< (std::ostream& os, const Connection& c);

}; // namespace LibKafka

#endif /* CONNECTION_H */
