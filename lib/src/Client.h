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

#ifndef CLIENT_H
#define CLIENT_H

#include <iostream>
#include <string>
#include <Connection.h>
#include <Request.h>
#include <Response.h>
#include <Debug.h>
#include <MetadataRequest.h>
#include <MetadataResponse.h>

namespace LibKafka {

class Client
{
  public:

    Client(std::string brokerHost, int brokerPort);
    ~Client();

    MetadataResponse *sendMetadataRequest(MetadataRequest *request);

  protected:

    Connection *connection;
    std::string brokerHost;
    int brokerPort;

    void prepareConnection();
    int sendRequest(Request *request);
    template <typename ResponseClass> ResponseClass *receiveResponse();
    template <typename RequestClass, typename ResponseClass> ResponseClass *apiCall(RequestClass *request);
};

}; // namespace LibKafka

#endif /* CONNECTION_H */
