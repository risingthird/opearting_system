SHELL:=/bin/bash -O extglob


mem.o : mem.c util.h
	gcc -Wall -g -c mem.c

libusermem.so : usermem.o
	g++ -o libusermem.so usermem.o -shared -g

simple_8_byte_allocation: simple_8_byte_allocation.c mem.o
	g++ -o simple_8_byte_allocation simple_8_byte_allocation.c mem.so -g


clean: 
	@rm -f !(*.cpp|Makefile|README.md|*.h|BMC-CS355|*.c|*.txt)