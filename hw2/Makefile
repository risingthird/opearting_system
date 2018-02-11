SHELL:=/bin/bash -O extglob
objects = matrix.c shell_hist.c shell.c


all: $(objects)
	@gcc -pthread matrix.c -o matrix && gcc shell_hist.c -o shell_hist && gcc shell.c -o shell

matrix : matrix.c
	@gcc matrix.c -o matrix

shell_hist: shell_hist.c
	@gcc shell_hist.c -o shell_hist

shell: shell.c
	@gcc shell.c -o shell

clean:
	@rm -f !(*.c|Makefile|README.md)
