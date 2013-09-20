###
### RPM spec file for libkafka
###
%define RPM_DIR rpm
%define _noarch_libdir /usr/lib
%define rel_ver 0.2.0
%define _topdir %(echo $PWD)/%{RPM_DIR}
%define _tmppath %(echo $PWD)/%{RPM_DIR}

Summary: A C++ client library for Apache Kafka v0.8+. Also includes C API.
Name: libkafka
Provides: libkafka
Version: %{rel_ver}
Release: 1
License: Apache License v2.0
Group: Applications/Databases
Source0: %{name}-%{version}.tar.bz2
URL: http://github.com/adobe-research/libkafka
Vendor: Adobe Systems, Inc.
Packager: David Tompkins <tompkins_at_adobe_dot_com>
BuildRoot: %{RPM_DIR}
Requires: gtest, zlib
BuildRequires: gtest-dev, zlib-devel, libtool
AutoReqProv: no

%description
A C++ client library for Apache Kafka v0.8+. Also includes C API. With a full unit test suite.

Supports the following Kafka API request/response calls:

-- Metadata
-- Produce
-- Fetch
-- Offset

Not compatible with Kafka versions prior to 0.8 due to wire protocol changes.

%prep
%setup -n %{name}-%{version}

%build
./configure --prefix=/usr --enable-gtest
make

%install
make ROOT="$RPM_BUILD_ROOT" DESTDIR=%{buildroot} install

%clean
rm -rf %{buildroot}
rm -rf $RPM_BUILD_ROOT

%files
%doc AUTHORS
%doc COPYING
%doc README.md
%doc CHANGELOG
%doc LICENSE
/usr/lib/libkafka.la
/usr/lib/libkafka.a
%ifos Linux
/usr/lib/libkafka.so*
%endif
%ifos darwin
/usr/lib/libkafka*dylib
%endif
/usr/include/libkafka/ApiConstants.h
/usr/include/libkafka/Client.h
/usr/include/libkafka/Connection.h
/usr/include/libkafka/Debug.h
/usr/include/libkafka/Packet.h
/usr/include/libkafka/PacketWriter.h
/usr/include/libkafka/Request.h
/usr/include/libkafka/RequestOrResponse.h
/usr/include/libkafka/Response.h
/usr/include/libkafka/Message.h
/usr/include/libkafka/MessageSet.h
/usr/include/libkafka/TopicNameBlock.h
/usr/include/libkafka/Util.h
/usr/include/libkafka/WireFormatter.h
/usr/include/libkafka/metadata/MetadataRequest.h
/usr/include/libkafka/metadata/MetadataResponse.h
/usr/include/libkafka/metadata/Broker.h
/usr/include/libkafka/metadata/PartitionMetadata.h
/usr/include/libkafka/metadata/TopicMetadata.h
/usr/include/libkafka/produce/ProduceRequest.h
/usr/include/libkafka/produce/ProduceMessageSet.h
/usr/include/libkafka/produce/ProduceResponse.h
/usr/include/libkafka/produce/ProduceResponsePartition.h
/usr/include/libkafka/fetch/FetchRequest.h
/usr/include/libkafka/fetch/FetchPartition.h
/usr/include/libkafka/fetch/FetchResponse.h
/usr/include/libkafka/fetch/FetchResponsePartition.h
/usr/include/libkafka/offset/OffsetRequest.h
/usr/include/libkafka/offset/OffsetPartition.h
/usr/include/libkafka/offset/OffsetResponse.h
/usr/include/libkafka/offset/OffsetResponsePartition.h
