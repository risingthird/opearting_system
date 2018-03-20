SHELL:=/bin/bash -O extglob




userthread.o : userthread.cpp util.h
	g++ -Wall -pedantic -fpic -g -c userthread.cpp

libuserthread.so : userthread.o
	g++ -o libuserthread.so userthread.o -shared -g


userthread_aging.o : userthread_aging.cpp util.h
	g++ -Wall -pedantic -fpic -g -c userthread_aging.cpp

libuserthread.so : userthread.o
	g++ -o libuserthread.so userthread.o -shared -g

basic_fifo: basic_fifo.c libuserthread.so
	g++ -o basic_fifo basic_fifo.c libuserthread.so -L. -luserthread -g

test_128_threads: test_128_threads.c libuserthread.so
	g++ -o test_128_threads test_128_threads.c libuserthread.so -L. -luserthread -g

test_join_chain: test_join_chain.c libuserthread.so
	g++ -o test_join_chain test_join_chain.c libuserthread.so -L -luserthread -g

test_sjf_yield: test_sjf_yield.c libuserthread.so
	g++ -o test_sjf_yield test_sjf_yield.c libuserthread.so -L -luserthread -g

test_priority_yield: test_priority_yield.c libuserthread.so
	g++ -o test_priority_yield test_priority_yield.c libuserthread.so -L -luserthread -g

test_misuse: test_misuse.c libuserthread.so
	g++ -o test_misuse test_misuse.c libuserthread.so -L -luserthread -g

nested_fifo_test: nested_fifo_test.c libuserthread.so
	g++ -o nested_fifo_test nested_fifo_test.c libuserthread.so -L -luserthread -g

test_create_within_thread: test_create_within_thread.c libuserthread.so
	g++ -o test_create_within_thread test_create_within_thread.c libuserthread.so -L -luserthread -g

intensive_priority: intensive_priority.c libuserthread.so
	g++ -o intensive_priority intensive_priority.c libuserthread.so -L -luserthread -g

priority_long_time: priority_long_time.c libuserthread.so
	g++ -o priority_long_time priority_long_time.c libuserthread.so -L -luserthread -g

fifo_join_non-existing: fifo_join_non-existing.c libuserthread.so
	g++ -o fifo_join_non-existing fifo_join_non-existing.c libuserthread.so -L -luserthread -g

fifo_null_func: fifo_null_func.c libuserthread.so
	g++ -o fifo_null_func fifo_null_func.c libuserthread.so -L -luserthread -g

clean:
	@rm -f !(*.cpp|Makefile|README.md|*.h|BMC-CS355|*.c|*.txt)