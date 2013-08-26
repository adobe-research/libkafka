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

}; // namespace LibKafka
