libkafka [![Build Status](https://travis-ci.org/adobe-research/libkafka.png)](https://travis-ci.org/adobe-research/libkafka) [![Coverage Status](https://coveralls.io/repos/adobe-research/libkafka/badge.png)](https://coveralls.io/r/adobe-research/libkafka)
========

A C++ client library for Apache Kafka v0.8+. Also includes C API. With a full unit test suite.

Supports the following Kafka API request/response calls:

* Metadata
* Produce
* Fetch
* Offset

Includes support for MessageSet compression using GZIP or Snappy compression.

Not compatible with Kafka versions prior to 0.8 due to wire protocol changes.

Version
=======
Current release version: 0.5.0 (see <a href="https://github.com/adobe-research/libkafka/releases">RELEASES</a>)

Dependencies
============

* zlib for CRC calculation, GZIP compression
* Snappy for Snappy compression
* Google Test for unit tests
* (optional) lcov/gcov for unit test coverage reporting
* (optional) doxygen for C/C++ API documentation

Installation
============

Supports both autoconf and maven builds:

Autoconf (g++)
* ./autogen.sh
* ./configure 
* make (builds shared library)
* make check (runs protocol unit tests, expects a kafka broker at host:port and with topics as specified in TestConfig.cc)
* make rpm (packages an RPM, tested on CentOs 6.x, genearted into ./rpm/RPMS/$(arch))
* make distcheck (produces release packages, tar.gz and tar.bz2)
* make maintainer-clean (clean all autoconf generated files)
* make rpm-clean (clean all artifacts of rpm, dist, and autotools)

Unit test coverage reports (gcov/lcov)
* ./autogen.sh && ./configure --enable-gcov && make && make check && make coverage-html
* HTML reports produced in ./coveragereport

Maven (clang)
* mvn clean install (builds shared library, command line apps)
* mvn test (runs protocol unit tests)

Debugging
* Make with -DDEBUG or uncomment -DDEBUG in the top level pom.xml, and rebuild for full protocol debugging output.

Valgrind
* Valgrind currently provides a clean memory leak report for the unit test suite. Future commits will maintain this.
* ./valgrind.sh runs the valgrind tests with suppressions for Mac OS X.

Examples
========

* Take a look at ClientTest.cc in test/src. There are current examples for Metadata, Produce, and Fetch.
* Take a look at the unit tests in test/src to see examples for any individual class.

TODO
====
* Advanced client with error handling, metadata/broker/leader updates, and offset management
* load testing for Produce and Fetch

Known issues
=============
* Nested MessageSet blocks are not yet handled. This is a wonky part of the protocol.
* The extern C wrrapper for C++ calls is not yet implemented.
* The .deb packaging remains broken. RPM packaging is fully operational.

Fixed Issues
============
* See [![CHANGELOG](CHANGELOG)](CHANGELOG)

License
============
Licensed under the Apache Software License 2.0. See [![LICENSE](LICENSE)](LICENSE) file.
