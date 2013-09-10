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

#include <string>
#include <ApiConstants.h>

class TestConfig
{
  public:

    const static std::string CONNECTION_HTTP_HOST;
    const static int CONNECTION_HTTP_PORT = 80;
    
    const static std::string CLIENT_BROKER_HOST;
    const static int CLIENT_BROKER_PORT = 9092;

    const static std::string PRODUCE_REQUEST_TOPIC_NAME;
    const static std::string PRODUCE_RESPONSE_TOPIC_NAME;

    const static std::string FETCH_RESPONSE_TOPIC_NAME;

    const static std::string OFFSET_REQUEST_TOPIC_NAME;
    const static std::string OFFSET_RESPONSE_TOPIC_NAME;
};
