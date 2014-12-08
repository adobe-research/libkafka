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
#include <typeinfo>

#if defined(WIN32)
#else
	#include <sys/socket.h>
	#include <arpa/inet.h>
	#include <netdb.h>
	#include <errno.h>
#endif

#include "Client.h"
#include "Util.h"
#include "Debug.h"

using namespace std;

namespace LibKafka {

Client::Client(string brokerHost, int brokerPort)
{
  D(cout.flush() << "--------------Client(" << brokerHost << ":" << intToString(brokerPort) << "\n";)

  this->brokerHost = brokerHost;
  this->brokerPort = brokerPort;
  this->connection = NULL; // lazy connect
}

Client::~Client()
{
  D(cout.flush() << "--------------~Client()\n";)

    if (connection != NULL) delete this->connection;
}

MetadataResponse *Client::sendMetadataRequest(MetadataRequest *request)
{
  return apiCall<MetadataRequest, MetadataResponse>(request);
}

ProduceResponse *Client::sendProduceRequest(ProduceRequest *request)
{
  return apiCall<ProduceRequest, ProduceResponse>(request);
}

FetchResponse *Client::sendFetchRequest(FetchRequest *request)
{
  return apiCall<FetchRequest, FetchResponse>(request);
}

OffsetResponse *Client::sendOffsetRequest(OffsetRequest *request)
{
  return apiCall<OffsetRequest, OffsetResponse>(request);
}

template <typename RequestClass, typename ResponseClass>
ResponseClass *Client::apiCall(RequestClass *request)
{
  D(cout.flush() << "--------------Client::apiCall():" << typeid(RequestClass).name() << "\n";)

  if (!this->prepareConnection()) { E("Client::apiCall():unable to create connection"); return NULL; }

  int status = this->sendRequest(request);
  if (status == Connection::WRITE_ERROR)
  {
    E("Client::apiCall():sendRequest() error:" << strerror(errno) << "\n");
    return NULL;
  }

  D(cout.flush() << "Client::apiCall:" << typeid(RequestClass).name() << " sent:\n" << *request;)

  ResponseClass *response = this->receiveResponse<ResponseClass>();
  if (response == NULL)
  {
    E("Client::apiCall():receiveResponse() error:" << strerror(errno) << "\n");
    return NULL;
  }

  D(cout.flush() << "Client::apiCall:" << typeid(ResponseClass).name() << " received:\n" << *response;)
  return response;
}

int Client::sendRequest(Request *request)
{
  D(cout.flush() << "--------------Client::sendRequest()\n";)
  D(cout.flush() << "Request:\n" << *request;)
    
  if (!this->prepareConnection()) { E("Client::sendRequest():unable to create connection"); return Connection::OPEN_CONNECTION_ERROR; }

  unsigned char *buffer = request->toWireFormat();
  int numBytesSent = this->connection->write(request->size(), buffer);
  if (numBytesSent == Connection::WRITE_ERROR) { E("Client::sendRequest():write error:" << strerror(errno) << "\n"); return numBytesSent; }
  D(cout.flush() << "Client::sendRequest():request sent:numBytes:" << numBytesSent << "\n";)
  return numBytesSent;
}

template <typename ResponseClass>
ResponseClass *Client::receiveResponse()
{
  D(cout.flush() << "--------------Client::receiveResponse()\n";)
  
  if (!this->prepareConnection()) { E("Client::receiveResponse():unable to create connection"); return NULL; }

  int netValueSize = -1;
  int numBytesReceived = this->connection->read(sizeof(int), (unsigned char *)(&netValueSize));
  if (numBytesReceived == Connection::READ_ERROR) { E("Client::receiveResponse():read error on size:" << strerror(errno) << "\n"); return NULL; }
  int hostValueSize = ntohl(netValueSize);
  D(cout.flush() << "Client::receiveResponse():incoming response:size:" << hostValueSize << "\n";)
  unsigned char *buffer = new unsigned char[hostValueSize+sizeof(int)]; // add space for int32 size
  memcpy(buffer, &netValueSize, sizeof(int));
  numBytesReceived = this->connection->read(hostValueSize, buffer + sizeof(int));
  if (numBytesReceived == Connection::READ_ERROR) { E("Client::receiveResponse():read error on body:" << strerror(errno) << "\n"); return NULL; }
  return new ResponseClass(buffer, true); // true specfies delete buffer on ~Response()
}

bool Client::prepareConnection()
{
  if (this->connection != NULL) return true;
  connection = new Connection(this->brokerHost, this->brokerPort);
  if (connection->open() < 0) { E("Client::prepareConnection():connection->open() failed\n"); return false; }
  return true;
}

}; // namespace LibKafka
