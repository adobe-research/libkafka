# Force usage of the latest version of CMake (2.8 or later), or fatally err out
# if we're on a system with an older installation (we're going to use features
# that were *just* implemented in CMake 2.8 to make things a bit easier.)
CMAKE_MINIMUM_REQUIRED(VERSION 2.8 FATAL_ERROR)

PROJECT(kafka)

# Turn on ablilty to create folders to organize vcproj files.
SET_PROPERTY(GLOBAL PROPERTY USE_FOLDERS ON)

# Set the Use of MFC flag (related to _AFXDLL). We have the following options:
# 0: Use Standard Windows Libraries (default)
# 1: Use MFC in a Static Library
# 2: Use MFC in a Shared DLL
SET(CMAKE_MFC_FLAG 0)

# Disable warnings about using insecure CRT functions
add_definitions( -D_CRT_SECURE_NO_WARNINGS )


# Try to find ZLIB
find_package(ZLIB)
if (ZLIB_FOUND)
    include_directories(${ZLIB_INCLUDE_DIRS})
else()
	message( FATAL_ERROR "Unable to file ZLIB" )
endif()

# Distinguish between debug and release lib
if (MSVC)
	if(CMAKE_SIZEOF_VOID_P EQUAL 8)
		set(CMAKE_DEBUG_POSTFIX "_w64d")
		set(CMAKE_RELEASE_POSTFIX "_w64r")
	else()
		set(CMAKE_DEBUG_POSTFIX "_wd")
		set(CMAKE_RELEASE_POSTFIX "_wr")
	endif()
endif()

include_directories(
	../..
	../../ZLib/inc
)

set(libkafka_HEADERS
	ApiConstants.h
	Client.h
	Connection.h
	Debug.h
	ErrorHandler.h
	fetch/FetchPartition.h
	fetch/FetchRequest.h
	fetch/FetchResponse.h
	fetch/FetchResponsePartition.h
	Message.h
	MessageSet.h
	metadata/Broker.h
	metadata/MetadataRequest.h
	metadata/MetadataResponse.h
	metadata/PartitionMetadata.h
	metadata/TopicMetadata.h
	offset/OffsetPartition.h
	offset/OffsetRequest.h
	offset/OffsetResponse.h
	offset/OffsetResponsePartition.h
	Packet.h
	PacketWriter.h
	produce/ProduceMessageSet.h
	produce/ProduceRequest.h
	produce/ProduceResponse.h
	produce/ProduceResponsePartition.h
	Request.h
	RequestOrResponse.h
	Response.h
	TopicNameBlock.h
	Util.h
	WireFormatter.h
)

set(libKafka_SOURCES
	ApiConstants.cc
	Client.cc
	Connection.cc
	fetch/FetchPartition.cc
	fetch/FetchRequest.cc
	fetch/FetchResponse.cc
	fetch/FetchResponsePartition.cc
	Message.cc
	MessageSet.cc
	metadata/Broker.cc
	metadata/MetadataRequest.cc
	metadata/MetadataResponse.cc
	metadata/PartitionMetadata.cc
	metadata/TopicMetadata.cc
	offset/OffsetPartition.cc
	offset/OffsetRequest.cc
	offset/OffsetResponse.cc
	offset/OffsetResponsePartition.cc
	Packet.cc
	produce/ProduceMessageSet.cc
	produce/ProduceRequest.cc
	produce/ProduceResponse.cc
	produce/ProduceResponsePartition.cc
	Request.cc
	RequestOrResponse.cc
	Response.cc
	Util.cc
)

list(APPEND libKafka_SOURCES_AND_HEADERS ${libKafka_SOURCES} ${libkafka_HEADERS})

add_library(kafka STATIC ${libKafka_SOURCES_AND_HEADERS})
