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

#ifdef __cpp
extern "C" {
#endif

#pragma pack(push, 1)

struct ProtocolString
{
  short int length;
  char byteArray[];
};

template <typename T>
struct ProtocolArray
{
  int length;
  T typeArray[];
};

struct RequestOrResponse
{
  int size;
};

struct RequestMessage : RequestOrResponse
{
  short int apiKey;
  short int apiVersion;
  int correlationId;
  ProtocolString clientId;
};

struct ResponseMessage : RequestOrResponse
{
  int correlationId;
};

struct MetaDataRequest : RequestMessage
{
  ProtocolArray<ProtocolString> topicNameArray;
};
typedef struct MetaDataRequest metadata_request;

struct Broker
{
  int nodeId;
  ProtocolString host;
  int port;
};

struct PartitionMetadata
{
  short int partitionErrorCode;
  int partitionId;
  int leader;
  ProtocolArray<int> replicaArray;
  ProtocolArray<int> isrArray;
};

struct TopicMetadata
{
  short int topicErrorCode;
  ProtocolString topicName;
  ProtocolArray<PartitionMetadata> partitionMetadataArray;
};

struct MetaDataResponse : ResponseMessage
{
  ProtocolArray<Broker> brokerArray;
  ProtocolArray<TopicMetadata> topicMetadataArray;
};
typedef struct MetaDataResponse metadata_response;

#pragma pack(pop)

// C API functions
metadata_request *create_metadata_request(char *topic_name);

#ifdef __cpp
}
#endif
