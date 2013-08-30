libkafka
========

A C++ client library for Apache Kafka. Also includes C API. With a full unit test suite.

Dependencies
============

* zlib for CRC calculation (yum install zlib / port install zlib)
* Google Test for unit tests (yum install gtest / port install google-test)
* (optional) Doxygen for C/C++ API documentation (yum install doxygen / port install doxygen)

Installation
============

Supports both autoconf and maven builds:

Maven
* mvn clean install (builds shared library, command line apps)
* mvn test (runs protocol unit tests)

Autoconf
* ./autogen.sh
* ./configure --enable-gtest (requires --enable-gtest due to Google Test m4 macro bug)
* make install (builds and installs shared library)
* make check (runs protocol unit tests)

And to clean all autoconf generated files:
* make maintainer-clean

Debugging
* uncomment -DDEBUG in the top level pom.xml, rebuild for full protocol debugging output.

Valgrind
* Valgrind currently provides a clean memory leak report for the unit test suite. Future commits will maintain this.

Examples
========

* Take a look at ClientTest.cc in test/src. There are current examples for Metadata, Produce, and Fetch.
* Take a look at the unit tests in test/src to see examples for any individual class.

TODO
====
* fix RPM creation for maven build
* load testing for Produce and Fetch

Known issues
=============
* There is an open bug with FetchResponse. (push ETA 8/30/2013).
* Nested MessageSet blocks are not yet handled. This is a wonky part of the protocol.
* The extern C wrrapper for C++ calls is not yet implemented.
* OffSetRequest is still buggy (push ETA 9/4/2013).
* RPM generation in the maven build is broken.
