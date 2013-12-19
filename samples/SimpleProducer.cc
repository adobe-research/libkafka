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
#include <vector>
#include <errno.h>
#include <libkafka/ApiConstants.h>
#include <libkafka/Client.h>
#include <libkafka/Message.h>
#include <libkafka/MessageSet.h>
#include <libkafka/TopicNameBlock.h>
#include <libkafka/produce/ProduceMessageSet.h>
#include <libkafka/produce/ProduceRequest.h>
#include <libkafka/produce/ProduceResponsePartition.h>
#include <libkafka/produce/ProduceResponse.h>

using namespace std;
using namespace LibKafka;

Message* createMessage(const char * value, const char *key = "");
ProduceRequest* createProduceRequest(string topic_name, Message **messageArray, int messageArraySize);

main()
{
  const char *hostname = "<your kafka deployment hostname here>";
  const int port = 9092;
  const char *topic_name = "test_topic";
  const char *message_string = "This is a test message.";

  Client *c = new Client(hostname, port);

  Message *m = createMessage(message_string);
  Message **messageArray = &m;

  ProduceRequest *request = createProduceRequest(topic_name, messageArray, 1);

  // optionally set compression mode, will be automatic when messages are sent
  //request->setCompression(ApiConstants::MESSAGE_COMPRESSION_GZIP);

  ProduceResponse *response = c->sendProduceRequest(request);

  if (response == NULL)
  {
    cerr << "an error ocurred while sending the produce request, errno = " << strerror(errno) << "\n";
  }
  else
  {
    if (response->hasErrorCode())
      cerr << "publish error detected\n";
    else
      cout << "message successfully published to kafka\n";
  }

  delete request;
  if (response != NULL) delete response;
}

// culled from high-level client (coming soon...)

vector<Message*> messageVector;
MessageSet* messageSet;
ProduceMessageSet** produceMessageSetArray;
TopicNameBlock<ProduceMessageSet>** produceTopicArray;

Message* createMessage(const char * value, const char *key)
{
  // these will be updated as the message is prepared for production
  const static int crc = 1001;
  const static signed char magicByte = -1;
  const static signed char attributes = 0; // last three bits must be zero to disable gzip compression

  unsigned char *v = new unsigned char[strlen(value)];
  memcpy(v, value, strlen(value));

  unsigned char *k = new unsigned char[strlen(key)];
  memcpy(k, key, strlen(key));

  return new Message(crc, magicByte, attributes, strlen(key), (unsigned char *)k, strlen(value), (unsigned char *)v, 0, true);
}

MessageSet* createMessageSet(Message **messageArray, int messageArraySize)
{
  int messageSetSize = 0;
  messageVector.clear();

  for (int i = 0 ; i < messageArraySize ; i++)
  {
    messageVector.push_back(messageArray[i]);
    // sizeof(offset) + sizeof(messageSize) + messageSize
    messageSetSize += sizeof(long int) + sizeof(int) + messageArray[i]->getWireFormatSize(false);
  }

  return new MessageSet(messageSetSize, messageVector, true);
}

ProduceMessageSet* createProduceMessageSet(Message **messageArray, int messageArraySize)
{
  messageSet = createMessageSet(messageArray, messageArraySize);
  int messageSetSize = messageSet->getWireFormatSize(false);
  // using partition = 0
  return new ProduceMessageSet(0, messageSetSize, messageSet, true);
}

TopicNameBlock<ProduceMessageSet>* createProduceRequestTopicNameBlock(string topic_name, Message **messageArray, int messageArraySize)
{
  const int produceMessageSetArraySize = 1;

  produceMessageSetArray = new ProduceMessageSet*[produceMessageSetArraySize];
  for (int i=0; i<produceMessageSetArraySize; i++) {
    produceMessageSetArray[i] = createProduceMessageSet(messageArray, messageArraySize);
  }
  return new TopicNameBlock<ProduceMessageSet>(topic_name, produceMessageSetArraySize, produceMessageSetArray, true);
}

ProduceRequest* createProduceRequest(string topic_name, Message **messageArray, int messageArraySize)
{
  const int correlationId = 212121;
  const string clientId = string("libkafka-test");
  const static int requiredAcks = 1;
  const static int timeout = 20;

  int produceTopicArraySize = 1;
  produceTopicArray = new TopicNameBlock<ProduceMessageSet>*[produceTopicArraySize];
  for (int i=0; i<produceTopicArraySize; i++) {
    produceTopicArray[i] = createProduceRequestTopicNameBlock(topic_name, messageArray, messageArraySize);
  }
  return new ProduceRequest(correlationId, clientId, requiredAcks, timeout, produceTopicArraySize, produceTopicArray, true);
}
