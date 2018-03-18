SHELL:=/bin/bash -O extglob


util.o : util.cpp
	g++ -Wall -pedantic -fpic -g -c util.cpp

userthread.o : userthread.cpp
	g++ -Wall -pedantic -fpic -g -c userthread.cpp

libuserthread.so : userthread.o util.o
	g++ -o libuserthread.so userthread.o util.o -shared -g




clean:
	@rm -f !(*.cpp|Makefile|README.md|*.h|BMC-CS355)