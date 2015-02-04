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

#ifndef UTIL_H
#define UTIL_H

#include <cstdint>
#include <string>

namespace LibKafka {

  std::string intToString(int i);
  std::string charToBinaryString(unsigned char x);
  std::string intArrayToString(int* i, int size);
  std::string longIntArrayToString(int64_t* i, int size);

}; // namespace LibKafka

#ifndef ntohll
#define ntohll(x) ( ( (uint64_t)(ntohl( (uint32_t)((x << 32) >> 32) )) << 32) | ntohl( ((uint32_t)(x >> 32)) ) )                                        
#define htonll(x) ntohll(x)
#endif

#ifndef MAX
#define MAX(x, y) ( ((x) > (y)) ? (x) : (y) )
#endif

#endif /* UTIL_H */
