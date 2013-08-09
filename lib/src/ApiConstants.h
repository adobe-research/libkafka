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

// API request key values
#define ProduceRequest 		0
#define FetchRequest 		1
#define OffsetRequest 		2
#define MetadataRequest 	3
#define LeaderAndIsrRequest 	4
#define StopReplicaRequest 	5
#define OffsetCommitRequest 	6
#define OffsetFetchRequest 	7 

// API error codes

// No error--it worked! 
#define NoError				0
// An unexpected server error 
#define Unknown  	       		-1
// The requested offset is outside the range of offsets maintained by the server for the given topic/partition. 
#define OffsetOutOfRange  		1
// This indicates that a message contents does not match its CRC 
#define InvalidMessage  		2
// This request is for a topic or partition that does not exist on this broker. 
#define UnknownTopicOrPartition		3
// The message has a negative size 
#define InvalidMessageSize  		4
// This error is thrown if we are in the middle of a leadership election and there is currently no leader for this partition and hence it is unavailable for writes. 
#define LeaderNotAvailable  		5
// This error is thrown if the client attempts to send messages to a replica that is not the leader for some partition. It indicates that the clients metadata is out of date. 
#define NotLeaderForPartition  		6
// This error is thrown if the request exceeds the user-specified time limit in the request. 
#define RequestTimedOut  		7
// This is not a client facing error and is used only internally by intra-cluster broker communication. 
#define BrokerNotAvailable  		8
// What is the difference between this and LeaderNotAvailable? 
#define ReplicaNotAvailable  		9
// The server has a configurable maximum message size to avoid unbounded memory allocation. This error is thrown if the client attempt to produce a message larger than this maximum. 
#define MessageSizeTooLarge  		10
// ??? 
#define StaleControllerEpochCode  	11
// If you specify a string larger than configured maximum for offset metadata  
#define OffsetMetadataTooLargeCode  	12
