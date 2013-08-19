libkafka
========

A C++ client libray for Apache Kafka. Also includes C API.

Dependencies
============

* Google Test (yum install gtest or port install google-test)
* (optional) Doxygen for C/C++ API documentation (yum install doxygen or port install doxygen)

Installation
============

Supports both autoconf and maven builds:

Maven
* mvn clean install (builds shared library, command line apps)
* mvn test (runs protocol unit tests)

Autoconf
* ./configure --enable-gtest (requires --enable-gtest due to Google Test m4 macro bug)
* make install (builds and installs shared library)
* make check (runs protocol unit tests)

Autoconf changes require the following:
* ./autogen.sh

And to clean all autoconf generated files:
* make maintainer-clean
