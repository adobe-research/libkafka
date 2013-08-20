#valgrind --dsymutil=yes --gen-suppressions=all --leak-check=yes --suppressions=valgrind.supp test/target/libkafka-test.a
valgrind --dsymutil=yes --leak-check=yes --suppressions=valgrind.supp test/target/libkafka-test.a
