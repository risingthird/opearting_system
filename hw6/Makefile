SHELL:=/bin/bash -O extglob


defrag : defrag.c
	@gcc -o defrag -g defrag.c

clean: 
	@rm -f !(*.c|Makefile|README.md|*.h|datafile-frag*|large-frag|large-frag-defrag-data)