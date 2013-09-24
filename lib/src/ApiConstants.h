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

#ifndef APICONSTANTS_H
#define APICONSTANTS_H

namespace LibKafka {

  class ApiConstants
  {
    public:

      // API version key values
      const static int API_VERSION = 0;

      // API request key values
      const static int PRODUCE_REQUEST_KEY = 0;
      const static int FETCH_REQUEST_KEY = 1;
      const static int OFFSET_REQUEST_KEY = 2;
      const static int METADATA_REQUEST_KEY = 3;
      const static int LEADER_AND_ISR_REQUEST_KEY = 4;
      const static int STOP_REPLICA_REQUEST_KEY = 5;
      const static int OFFSET_COMMIT_REQUEST_KEY = 6;
      const static int OFFSET_FETCH_REQUEST_KEY = 7;

      // Message compression attribute values
      const static signed char MESSAGE_COMPRESSION_NONE = 0x00;
      const static signed char MESSAGE_COMPRESSION_GZIP = 0x01;
      const static signed char MESSAGE_COMPRESSION_SNAPPY = 0x02;

      // API error codes
      const static int ERRORCODE_UNKNOWN = -1;
      const static int ERRORCODE_NO_ERROR = 0;
      const static int ERRORCODE_OFFSET_OUT_OF_RANGE = 1;
      const static int ERRORCODE_INVALID_MESSAGE = 2;
      const static int ERRORCODE_UNKNOWN_TOPIC_OR_PARTITION = 3;
      const static int ERRORCODE_INVALID_MESSAGE_SIZE = 4;
      const static int ERRORCODE_LEADER_NOT_AVAILABLE = 5;
      const static int ERRORCODE_NOT_LEADER_FOR_PARTITION = 6;
      const static int ERRORCODE_REQUEST_TIMED_OUT = 7;
      const static int ERRORCODE_BROKER_NOT_AVAILABLE = 8;
      const static int ERRORCODE_REPLICA_NOT_AVAILABLE = 9;
      const static int ERRORCODE_MESSAGE_SIZE_TOO_LARGE = 10;
      const static int ERRORCODE_STALE_CONTROLLER_EPOCH_CODE = 11;
      const static int ERRORCODE_OFFSET_METADATA_TOO_LARGE_CODE = 12;
      const static int ERRORCODE_MINIMUM = -1;
      const static int ERRORCODE_MAXIMUM = 12;

      // API error strings
      const static char* ERRORSTRING_INVALID_ERROR_CODE;
      const static char* ERRORSTRING_UNKNOWN;
      const static char* ERRORSTRING_NO_ERROR;
      const static char* ERRORSTRING_OFFSET_OUT_OF_RANGE;
      const static char* ERRORSTRING_INVALID_MESSAGE;
      const static char* ERRORSTRING_UNKNOWN_TOPIC_OR_PARTITION;
      const static char* ERRORSTRING_INVALID_MESSAGE_SIZE;
      const static char* ERRORSTRING_LEADER_NOT_AVAILABLE;
      const static char* ERRORSTRING_NOT_LEADER_FOR_PARTITION;
      const static char* ERRORSTRING_REQUEST_TIMED_OUT;
      const static char* ERRORSTRING_BROKER_NOT_AVAILABLE;
      const static char* ERRORSTRING_REPLICA_NOT_AVAILABLE;
      const static char* ERRORSTRING_MESSAGE_SIZE_TOO_LARGE;
      const static char* ERRORSTRING_STALE_CONTROLLER_EPOCH_CODE;
      const static char* ERRORSTRING_OFFSET_METADATA_TOO_LARGE_CODE;

      const static char *getErrorString(int errorCode);

    protected:

      const static char *errorStringLookupTable[];
  };
};

#endif
