Samples
=======

A few high-level samples that handle all the low-level details of building request message structures.

I have received quite a few requests for a high-level producer/consumer API that is an analog of the Java high-level API, and I will be providing a high-level API in the next major libkafka release. In the interim, I have culled a few samples from that work-in-progress.

Compilation
=======
I tried to make each sample a standalone compile, e.g.:

     g++ SimpleProducer.cc -L/usr/lib -lz -lsnappy -lkafka

The sample code and the compile command above assume you have installed libkafka via the RPM, with the includes in /usr/include/libkafka and the shared library in /usr/lib. Also note the runtime library dependencies on zlib, snappy, and the libkafka library.

You will need to set hostname and topic_name variables to something that makes sense for your kafka deployment. For samples that produce messages to clustered topics, the hostname variable should be the hostname of the broker that is specified as the leader for the specified topic.

License
============
Licensed under the Apache Software License 2.0. See <a href="https://github.com/adobe-research/libkafka/LICENSE">LICENSE</a></a> file.
