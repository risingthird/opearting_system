SHELL:=/bin/bash -O extglob




mem.o : mem.c util.h
	gcc -Wall -pedantic -fpic -g -c mem.c

libusermem.so : mem.o
	gcc -o libusermem.so mem.o -shared -g

simple_8_byte_allocation: simple_8_byte_allocation.c libusermem.so
	gcc -o simple_8_byte_allocation simple_8_byte_allocation.c -L. -lusermem -g

check_for_8_byte_alignment: check_for_8_byte_alignment.c libusermem.so
	gcc -o check_for_8_byte_alignment check_for_8_byte_alignment.c -L. -lusermem -g


clean: 
	@rm -f !(*.cpp|Makefile|README.md|*.h|BMC-CS355|*.c|*.txt)