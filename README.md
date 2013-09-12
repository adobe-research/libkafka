libkafka <span width="100%"/></span><a href="https://travis-ci.org/adobe-research/libkafka"></a><img style="float:right" src="https://travis-ci.org/adobe-research/libkafka.png"/></a>
========

A C++ client library for Apache Kafka v0.8+. Also includes C API. With a full unit test suite.

Supports the following Kafka API request/response calls:

* Metadata
* Produce
* Fetch
* Offset

Not compatible with Kafka versions prior to 0.8 due to wire protocol changes.

Version
=======
Current release version: 0.1.0

Dependencies
============

* zlib for CRC calculation (yum install zlib / port install zlib)
* Google Test for unit tests (yum install gtest gtest-devel / port install google-test)
* (optional) Doxygen for C/C++ API documentation (yum install doxygen / port install doxygen)

Installation
============

Supports both autoconf and maven builds:

Autoconf (g++)
* ./autogen.sh
* ./configure --enable-gtest (requires --enable-gtest due to Google Test m4 macro bug)
* make && make install (builds and installs shared library)
* make check (runs protocol unit tests)
* DISTCHECK_CONFIGURE_FLAGS=--enable-gtest make distcheck (produces release packages, tar.gz and tar.bz2)
* make maintainer-clean (clean all autoconf generated files)

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
* fix RPM creation for maven build
* load testing for Produce and Fetch
* add gzip compression support for MessageSets

Known issues
=============
* Nested MessageSet blocks are not yet handled. This is a wonky part of the protocol.
* The extern C wrrapper for C++ calls is not yet implemented.
* RPM generation in the maven build is broken.

Fixed Issues
============
* See [![CHANGELOG](CHANGELOG)](CHANGELOG)

License
============
Licensed under the Apache Software License 2.0. See [![LICENSE](LICENSE)](LICENSE) file.
