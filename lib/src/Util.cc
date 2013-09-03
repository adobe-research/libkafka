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
#include <sstream>
#include "Util.h"

using namespace std;

namespace LibKafka {

string intToString(int i)
{
  ostringstream ss;
  ss << i;
  return ss.str();
}

string intArrayToString(int* array, int size)
{
  ostringstream ss;
  ss << "[";
  for (int i=0; i<size-1; i++)
    ss << array[i] << ",";
  ss << array[size-1];
  ss << "]";
  return ss.str();
}

}; // namespace LibKafka
