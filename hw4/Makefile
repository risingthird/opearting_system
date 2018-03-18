SHELL:=/bin/bash -O extglob


util.o : util.cpp
	g++ -Wall -pedantic -fpic -g -c util.cpp

userthread.o : userthread.cpp
	g++ -Wall -pedantic -fpic -g -c userthread.cpp

libuserthread.so : userthread.o util.o
	g++ -o libuserthread.so userthread.o util.o -shared -g

basic_fifo: basic_fifo.c libuserthread.so
	g++ basic_fifo basic_fifo.c libuserthread.so -L -luserthread -g

test_128_threads: test_128_threads.c libuserthread.so
	g++ test_128_threads test_128_threads.c libuserthread.so -L -luserthread -g

test_join_chain: test_join_chain.c libuserthread.so
	g++ test_join_chain test_join_chain.c libuserthread.so -L -luserthread -g

test_sjf_yield: test_sjf_yield.c libuserthread.so
	g++ test_sjf_yield test_sjf_yield.c libuserthread.so -L -luserthread -g

test_priority_yield: test_priority_yield.c libuserthread.so
	g++ test_priority_yield test_priority_yield.c libuserthread.so -L -luserthread -g

test_misuse: test_misuse.c libuserthread.so
	g++ test_misuse test_misuse.c libuserthread.so -L -luserthread -g

nested_fifo_test: nested_fifo_test.c libuserthread.so
	g++ nested_fifo_test nested_fifo_test.c libuserthread.so -L -luserthread -g

clean:
	@rm -f !(*.cpp|Makefile|README.md|*.h|BMC-CS355)