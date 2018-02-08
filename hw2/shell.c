#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define MAXLENGTH 256
#define STDIN 0
#define STDOUT 0
#define DEFAULT_NUMARG 32

void myPrompt();

int main(int argc, char **argv) {
	char* commandLine;
	char** args;
	int length;
	char *split;
	int numArg;
	int i;
	int counter;
	while(1) {
		myPrompt();
		length = MAXLENGTH;
		commandLine = malloc(sizeof(char) * length);
		if(fgets(commandLine[0], length, STDIN) == NULL) {
			free(commandLine);
			exit(0);
		}
		while(commandLine[length-1] != '\n' && commandLine[length-1] != NULL) {
			int loc = length;
			length = strlen(commandLine) + MAXLENGTH;
			commandLine = realloc(commandLine, length);
			fgets(commandLine[loc-1], length, STDIN)
		}

		if(strtok(commandLine, " \n\t") == NULL) continue;
		counter = 0;
		i = 0;
		args = malloc(sizeof(char*) * DEFAULT_NUMARG);
		while(split != NULL){
			split = strtok(NULL, " \n");
			args[counter++] = split;
			if(counter == (DEFAULT_NUMARG << i)) {
				i++;
				args = realloc(args, (DEFAULT_NUMARG << i));
			}
		}
		numArg = counter;
	}
}

void callChild(int cargc, char **argv) {
	pid_t pid;
	int status;
	if(cargc == 1 && !strcmp(argv[0], "exit")) {
		printf("Exit to main shell in a second!\n");
		sleep(1);
		exit(0);
	}
	pid = fork();
	if(pid == -1) {
		printf("Fork error occured! Program terminate in one second!\n");
		exit(0);
	}
	else if(pid == 0) {
		if(execvp(args[0], args)<0) { 
			printf("Cannot execute command! Exit in a second!\n");
			exit(1);
		}
		exit(0);
	}
	else {
		while(!wait(&status));
	}
}


void myPrompt() {
	char prompt[MAXLENGTH];
	if(getcwd(prompt, MAXLENGTH) == NULL){
        printf("Cannot get current path! Exit in one second!\n");
        sleep(1);
        exit(1);
    }
    printf("%s >>>>>>>>", prompt);
}