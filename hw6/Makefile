SHELL:=/bin/bash -O extglob

gcc -o defrag -g defrag.c

defrag : defrag.c
	@gcc defrag.c -o -g defrag

clean: 
	@rm -f !(*.c|Makefile|README.md|*.h)