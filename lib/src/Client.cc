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
#include <arpa/inet.h>
#include <netdb.h>
#include <errno.h>

#include <Client.h>
#include <Util.h>
#include <Debug.h>

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
  D(cout.flush() << "--------------Client::MetadataRequest()\n";)
  D(cout.flush() << "MetadataRequest:request:\n" << *request;)

  this->prepareConnection();

  int status = this->sendRequest(request);
  if (status == Connection::WRITE_ERROR)
  {
    E("Client::sendMetadataRequest():sendRequest() error:" << strerror(errno) << "\n");
    return NULL;
  }

  D(cout.flush() << "Client::sendMetadataRequest:request sent:\n" << *request;)

  MetadataResponse *response = this->receiveResponse<MetadataResponse>();
  if (response == NULL)
  {
    E("Client::sendMetadataRequest():receiveResponse() error:" << strerror(errno) << "\n");
    return NULL;
  }

  D(cout.flush() << "Client::sendMetadataRequest:response received:\n" << *response;)
  return response;
}

int Client::sendRequest(Request *request)
{
  D(cout.flush() << "--------------Client::sendRequest()\n";)
  D(cout.flush() << "Request:\n" << *request;)
    
  this->prepareConnection();
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
  
  this->prepareConnection();
  int netValueSize = -1;
  int numBytesReceived = this->connection->read(sizeof(int), (unsigned char *)(&netValueSize));
  if (numBytesReceived == Connection::READ_ERROR) { E("Client::receiveResponse():read error on size:" << strerror(errno) << "\n"); return NULL; }
  int hostValueSize = ntohl(netValueSize);
  D(cout.flush() << "Client::receiveResponse():incoming response:size:" << hostValueSize << "\n";)
  unsigned char *buffer = new unsigned char[hostValueSize];
  memcpy(buffer, &netValueSize, sizeof(int));
  numBytesReceived = this->connection->read(hostValueSize - sizeof(int), buffer + sizeof(int));
  if (numBytesReceived == Connection::READ_ERROR) { E("Client::receiveResponse():read error on body:" << strerror(errno) << "\n"); return NULL; }
  return new ResponseClass(buffer, true); // true specfies delete buffer on ~Response()
}

void Client::prepareConnection()
{
  if (this->connection != NULL) return;
  connection = new Connection(this->brokerHost, this->brokerPort);
  connection->open();
}

}; // namespace LibKafka
