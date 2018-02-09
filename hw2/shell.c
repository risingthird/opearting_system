#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define MAXLENGTH 2
#define STDIN 0
#define STDOUT 0
#define DEFAULT_NUMARG 1
#define HISTSIZE 50

char** args;
char* commandLine;
unsigned int numHist = 0;

void myPrompt();
void callChild(int cargc, char **argv);
void addToTail(histCommand* toAdd);
void popFront();
void destroyNode(histCommand* current);
void clearList();
histCommand* findFirstN(int n);
histCommand*  findLastN(int n);
histCommand* createNode(char** args, int numArg);

int main(int argc, char **argv) {
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
				commandLine = realloc(commandLine, length/2+1);
				commandLine[index] = '\0';
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
		

		//if((split = strtok(commandLine, " \n\t")) == NULL) continue;
		//counter = 0;

		//i = 0;
		//args = malloc(sizeof(char*) * DEFAULT_NUMARG);
		//if(!args) {
					//printf("Malloc failed\n");
					//exit(1);
		//}
		//strcpy(commandLine, temp_commandLine);
		if((split = strtok(commandLine, " \n\t")) == NULL) continue;
		counter = 0;

		i = 0;
		args = (char**) malloc(sizeof(char*) * DEFAULT_NUMARG);
		bzero(args, DEFAULT_NUMARG);
		int argLength = DEFAULT_NUMARG;
		while(split != NULL){
			//split = strtok(NULL, " \n\t");
			args[counter] = split;
			counter++;
			split = strtok(NULL, " \n\t");
			//int argLength = DEFAULT_NUMARG << i;
			if(counter >= argLength) {
				i++;
				argLength = DEFAULT_NUMARG << i;
				args = realloc(args, argLength * sizeof(char**));
			}
		}
		numArg = counter;
		for(int j = numArg; j < (DEFAULT_NUMARG<<i); j++) {
			args[j] = NULL;
		}
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
			free(args);
			free(commandLine);
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

void addToTail(histCommand* toAdd) {
	if(numHist <= HISTSIZE) {
		histCommand* temp = tail->prev;
		temp->next = toAdd;
		toAdd->prev = temp;
		toAdd->next = tail;
		tail->prev = toAdd;
		numHist++;
	}
	else {
		popFront();
		histCommand* temp = tail->prev;
		temp->next = toAdd;
		toAdd->prev = temp;
		toAdd->next = tail;
		tail->prev = toAdd;
		numHist++;
	}

}



void popFront() {
	if(head->next == NULL) {
		printf("No history commands!\n");
		return;
	}
	else if((head->next)->next == NULL) {
		destroyNode(head->next);
		head->next = NULL;
		tail->prev = NULL;
		numHist--;
		return;
	}
	
	histCommand* first = head->tail;
	histCommand* second = first->next;
	head->next = second;
	second->prev = head;
	destroyNode(first);
	numHist--;
	return;
}

histCommand* findFirstN(int n) {
	if(n > numHist) {
		printf("Sorry! No such histroy command!\n");
		exit(0);
	}
	else if(n == 1) {
		return head->next;
	}
	else {
		histCommand* temp = head;
		for(int i = 0; i < n; i++) {
			temp = temp->next;
		}
		return temp;
	}
}

histCommand*  findLastN(int n) {
	if(n > numHist) {
		printf("Sorry! No such histroy command!\n");
		exit(0);
	}
	else if(n == 1) {
		return tail->prev;
	}
	else {
		histCommand* temp = tail;
		for(int i = 0; i < n; i++) {
			temp = tail->prev;
		}
		return temp;
	}
}

void destroyNode(histCommand* current) {
	free(current->args);
	free(current);
	return;
}

void clearList() {
	if(!head->next) {
		return;
	}
	else {
		histCommand* temp = head->next;
		while(head->next->args) {
			head->next = temp->next;
			destroyNode(temp);
			temp = head->next;
		}
	}
}
