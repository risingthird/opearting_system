SHELL:=/bin/bash -O extglob


util.o: util.c
	gcc -Wall -pedantic -fpic -g -c util.c

mem.o : mem.c util.o
	gcc -Wall -pedantic -fpic -g -c mem.c

libusermem.so : mem.o
	gcc -o libusermem.so mem.o -shared -g

simple_8_byte_allocation: simple_8_byte_allocation.c libusermem.so
	gcc -o simple_8_byte_allocation simple_8_byte_allocation.c -L. -lusermem -g


clean: 
	@rm -f !(*.cpp|Makefile|README.md|*.h|BMC-CS355|*.c|*.txt)