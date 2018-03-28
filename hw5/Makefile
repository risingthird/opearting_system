SHELL:=/bin/bash -O extglob


usermem.o : mem.c util.h
	gcc -Wall -pedantic -fpic -g -c mem.c

libusermem.so : usermem.o
	g++ -o libusermem.so usermem.o -shared -g

basic_fifo: simple_8_byte_allocation.c libusermem.so
	g++ -o simple_8_byte_allocation simple_8_byte_allocation.c libusermem.so -L. -lusermem -g

usermem: mem.c util.h
	gcc -Wall -pedantic -g -o mem.c

clean: 
	@rm -f !(*.cpp|Makefile|README.md|*.h|BMC-CS355|*.c|*.txt)