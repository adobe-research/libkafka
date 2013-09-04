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

#ifndef CLIENT_H
#define CLIENT_H

#include <iostream>
#include <string>

#include "Connection.h"
#include "Request.h"
#include "Response.h"
#include "Debug.h"
#include "metadata/MetadataRequest.h"
#include "metadata/MetadataResponse.h"
#include "produce/ProduceRequest.h"
#include "produce/ProduceResponse.h"
#include "fetch/FetchRequest.h"
#include "fetch/FetchResponse.h"
#include "offset/OffsetRequest.h"
#include "offset/OffsetResponse.h"

namespace LibKafka {

class Client
{
  public:

    Client(std::string brokerHost, int brokerPort);
    ~Client();

    MetadataResponse *sendMetadataRequest(MetadataRequest *request);
    ProduceResponse *sendProduceRequest(ProduceRequest *request);
    FetchResponse *sendFetchRequest(FetchRequest *request);
    OffsetResponse *sendOffsetRequest(OffsetRequest *request);

  protected:

    Connection *connection;
    std::string brokerHost;
    int brokerPort;

    bool prepareConnection();
    int sendRequest(Request *request);
    template <typename ResponseClass> ResponseClass *receiveResponse();
    template <typename RequestClass, typename ResponseClass> ResponseClass *apiCall(RequestClass *request);
};

}; // namespace LibKafka

#endif /* CONNECTION_H */
