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

#include "ApiConstants.h"
#include "Debug.h"

namespace LibKafka {

const int ApiConstants::API_VERSION;

const int ApiConstants::PRODUCE_REQUEST_KEY;
const int ApiConstants::FETCH_REQUEST_KEY;
const int ApiConstants::OFFSET_REQUEST_KEY;
const int ApiConstants::METADATA_REQUEST_KEY;
const int ApiConstants::LEADER_AND_ISR_REQUEST_KEY;
const int ApiConstants::STOP_REPLICA_REQUEST_KEY;
const int ApiConstants::OFFSET_COMMIT_REQUEST_KEY;
const int ApiConstants::OFFSET_FETCH_REQUEST_KEY;

const int ApiConstants::ERRORCODE_MINIMUM;
const int ApiConstants::ERRORCODE_MAXIMUM;
const int ApiConstants::ERRORCODE_UNKNOWN;
const int ApiConstants::ERRORCODE_NO_ERROR;
const int ApiConstants::ERRORCODE_OFFSET_OUT_OF_RANGE;
const int ApiConstants::ERRORCODE_INVALID_MESSAGE;
const int ApiConstants::ERRORCODE_UNKNOWN_TOPIC_OR_PARTITION;
const int ApiConstants::ERRORCODE_INVALID_MESSAGE_SIZE;
const int ApiConstants::ERRORCODE_LEADER_NOT_AVAILABLE;
const int ApiConstants::ERRORCODE_NOT_LEADER_FOR_PARTITION;
const int ApiConstants::ERRORCODE_REQUEST_TIMED_OUT;
const int ApiConstants::ERRORCODE_BROKER_NOT_AVAILABLE;
const int ApiConstants::ERRORCODE_REPLICA_NOT_AVAILABLE;
const int ApiConstants::ERRORCODE_MESSAGE_SIZE_TOO_LARGE;
const int ApiConstants::ERRORCODE_STALE_CONTROLLER_EPOCH_CODE;
const int ApiConstants::ERRORCODE_OFFSET_METADATA_TOO_LARGE_CODE;

const char* ApiConstants::ERRORSTRING_INVALID_ERROR_CODE = "invalid error code";
const char* ApiConstants::ERRORSTRING_UNKNOWN = "unexpected server error";
const char* ApiConstants::ERRORSTRING_NO_ERROR = "no error";
const char* ApiConstants::ERRORSTRING_OFFSET_OUT_OF_RANGE = "offset out of range";
const char* ApiConstants::ERRORSTRING_INVALID_MESSAGE = "invalid message";
const char* ApiConstants::ERRORSTRING_UNKNOWN_TOPIC_OR_PARTITION = "unknown topic or partition";
const char* ApiConstants::ERRORSTRING_INVALID_MESSAGE_SIZE = "invalid message size";
const char* ApiConstants::ERRORSTRING_LEADER_NOT_AVAILABLE = "leader not available";
const char* ApiConstants::ERRORSTRING_NOT_LEADER_FOR_PARTITION = "not leader for partition";
const char* ApiConstants::ERRORSTRING_REQUEST_TIMED_OUT = "request timed out";
const char* ApiConstants::ERRORSTRING_BROKER_NOT_AVAILABLE = "broker not available";
const char* ApiConstants::ERRORSTRING_REPLICA_NOT_AVAILABLE = "replaice not available";
const char* ApiConstants::ERRORSTRING_MESSAGE_SIZE_TOO_LARGE = "message size too large";
const char* ApiConstants::ERRORSTRING_STALE_CONTROLLER_EPOCH_CODE = "stale controller epoch";
const char* ApiConstants::ERRORSTRING_OFFSET_METADATA_TOO_LARGE_CODE = "offset metadata too large";

const char* ApiConstants::getErrorString(int errorCode)
{
  if ((errorCode < ERRORCODE_MINIMUM) || (errorCode > ERRORCODE_MAXIMUM))
    return ERRORSTRING_INVALID_ERROR_CODE;

  if (errorCode == ERRORCODE_UNKNOWN)
  {
    return ERRORSTRING_UNKNOWN;
  }

  return errorStringLookupTable[errorCode];
}

const char *ApiConstants::errorStringLookupTable[] = {
  ERRORSTRING_NO_ERROR,
  ERRORSTRING_OFFSET_OUT_OF_RANGE,
  ERRORSTRING_INVALID_MESSAGE,
  ERRORSTRING_UNKNOWN_TOPIC_OR_PARTITION,
  ERRORSTRING_INVALID_MESSAGE_SIZE,
  ERRORSTRING_LEADER_NOT_AVAILABLE,
  ERRORSTRING_NOT_LEADER_FOR_PARTITION,
  ERRORSTRING_REQUEST_TIMED_OUT,
  ERRORSTRING_BROKER_NOT_AVAILABLE,
  ERRORSTRING_REPLICA_NOT_AVAILABLE,
  ERRORSTRING_MESSAGE_SIZE_TOO_LARGE,
  ERRORSTRING_STALE_CONTROLLER_EPOCH_CODE,
  ERRORSTRING_OFFSET_METADATA_TOO_LARGE_CODE
};

}; // namespace LibKafka
