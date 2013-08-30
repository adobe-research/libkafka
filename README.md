libkafka
========

A C++ client libray for Apache Kafka. Also includes C API. With a full unit test suite.

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

TODO
====
* add RPM creation to maven build
* load testing for Produce and Fetch
