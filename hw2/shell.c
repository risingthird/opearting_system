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
		int index = 0;
		myPrompt();
		length = MAXLENGTH;

		commandLine = malloc(sizeof(char) * length);
		if(!commandLine) {
			printf("Malloc failed\n");
			exit(1);
		}
		bzero(commandLine, length);

		char ch = 'A';
		while(ch != NULL && ch != '\n') {
			ch = getchar();
			commandLine[index] = ch;
			index++;
			if(index >= length) {
				length *= 2;
				char* temp = malloc(sizeof(char) * length);
				bzero(temp, length);
				if(!temp) {
					printf("Malloc failed\n");
					exit(1);
				}
				strcpy(temp, commandLine);
				free(commandLine);
				commandLine = temp;
			}
		}
		commandLine[index] = '\0';
		

		if((split = strtok(commandLine, " \n\t")) == NULL) continue;
		counter = 0;

		i = 0;
		args = malloc(sizeof(char*) * DEFAULT_NUMARG);
		if(!args) {
					printf("Malloc failed\n");
					exit(1);
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
