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

#ifndef CONNECTION_H
#define CONNECTION_H

#include <iostream>
#include <string>
#include <sys/socket.h>
#include <netdb.h>
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
