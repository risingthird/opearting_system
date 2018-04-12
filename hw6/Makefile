SHELL:=/bin/bash -O extglob


defrag : defrag.c
	@gcc -o defrag -g defrag.c

clean: 
	@rm -f !(*.c|Makefile|README.md|*.h)