#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define MAXLENGTH 16
#define STDIN 0
#define STDOUT 0
#define DEFAULT_NUMARG 32
char** args;
char* commandLine;

void myPrompt();
void callChild(int cargc, char **argv);

int main(int argc, char **argv) {
	//char* commandLine;
	//char** args;
	int length;
	char *split;
	int numArg;
	int i;
	int counter;
	while(1) {
		myPrompt();
		length = MAXLENGTH;
		char* temp_commandLine = malloc(sizeof(char) * MAXLENGTH);
		commandLine = malloc(sizeof(char) * length);
		bzero(commandLine, length);
		if(fgets(temp_commandLine, length, stdin) == NULL) {
			free(commandLine);
			free(temp_commandLine);
			exit(0);
		}
		int count_commandLine = 1;
		strcpy(commandLine, temp_commandLine);
		while(temp_commandLine[MAXLENGTH-2] != '\n' && temp_commandLine[MAXLENGTH-2] != NULL) {
			
			//strcpy(commandLine[length/2], temp_commandLine);
			int loc = length;
			if(count_commandLine * MAXLENGTH >= loc) {
				length *= 2;
				commandLine = realloc(commandLine, length);
			}
			//commandLine = realloc(commandLine, length);
			//bzero(commandLine, length);
			fgets(temp_commandLine, MAXLENGTH, stdin);
			strcpy(commandLine[MAXLENGTH * count_commandLine], temp_commandLine);
			count_commandLine++;
		}
		//strcpy(commandLine, temp_commandLine);
		if((split = strtok(commandLine, " \n\t")) == NULL) continue;
		counter = 0;

		i = 0;
		args = malloc(sizeof(char*) * DEFAULT_NUMARG);
		bzero(args, DEFAULT_NUMARG);
		while(split != NULL){
			//split = strtok(NULL, " \n\t");
			args[counter++] = split;
			split = strtok(NULL, " \n\t");
			if(counter == (DEFAULT_NUMARG << i)) {
				i++;
				args = realloc(args, (DEFAULT_NUMARG << i));
			}
		}
		numArg = counter;
		args[(DEFAULT_NUMARG << i)-1] = NULL;
		callChild(numArg, args);
		free(args);
		free(commandLine);
	}
	return 0;
}

void callChild(int cargc, char **argv) {
	pid_t pid;
	int status;
	if(cargc == 1 && !strcmp(argv[0], "exit")) {
		printf("Exit to main shell in a second!\n");
		free(args);
		free(commandLine);
		sleep(1);
		exit(0);
	}
	pid = fork();
	if(pid == -1) {
		printf("Fork error occured! Program terminate in one second!\n");
		free(args);
		free(commandLine);
		exit(0);
	}
	else if(pid == 0) {
		if(execvp(argv[0], argv)<0) { 
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
	char prompt[256];
	if(getcwd(prompt, 256) == NULL){
        printf("Cannot get current path! Exit in one second!\n");
        sleep(1);
        exit(1);
    }
    printf("%s >>>>>>>>", prompt);
}
