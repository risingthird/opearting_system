SHELL:=/bin/bash -O extglob


defrag : defrag.c
	@gcc defrag.c -o -g defrag

clean: 
	@rm -f !(*.c|Makefile|README.md|*.h)