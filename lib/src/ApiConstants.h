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
