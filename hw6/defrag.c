#include "defrag.h"





int main(int argc, char** argv) {
	char file_name[100];
	
	if (argc != 2) {
		exit(EXIT_ERROR);
	}

	if (strlen(argv[1]) == 2) {
		if (argv[1][0] == '-') {
			if (argv[1][1] == 'h') {
				print_usage();
				exit(EXIT_SUCCESS);
			}
			else {
				exit(EXIT_ERROR);
			}
		}
	}

	if (argv[1][0] != '-') {
		strcpy(file_name, argv[1]);
	}

	in_file = fopen(file_name, 'r');
	printf("I died in 31\n");
	out_file = fopen(strcat(file_name, "-defrag"), 'w');
	printf("I died in 33\n");

	fseek(in_file, 0, SEEK_END);
	int malloc_size = ftell(in_file);
	fseek(in_file, 0, SEEK_SET);
	whole_buffer = malloc(malloc_size + 1);
	fread(whole_buffer, 1, malloc_size, in_file);

	sb = (SuperBlock*) (whole_buffer + BOOT_SIZE);
	printf("%d\n", sb->size);




}


void print_usage() {}

















































































