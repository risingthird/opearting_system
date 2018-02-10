#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define MAXLENGTH 2
#define STDIN 0
#define STDOUT 0
#define DEFAULT_NUMARG 1
#define HISTSIZE 50

typedf struct prevCommand
{
	int numArg;
	char** args;
	char* command;
	struct prevCommand* prev;
	struct prevCommand* next;
} histCommand;


char** args;
char* commandLine;
unsigned int numHist;
histCommand* head;
histCommand* tail;

void myPrompt();
void callChild(int cargc, char **argv);
void addToTail(histCommand* toAdd);
void popFront();
void destroyNode(histCommand* current);
void clearList();
void showHistory();
int checkHist(char* toCheck);
int isBeginWith(const char * str1, char *str2)
histCommand* findFirstN(int n);
histCommand*  findLastN(int n);
histCommand* createNode(char** args, int numArg);


int main(int argc, char **argv) {
	int length;
	char *split;
	int numArg;
	int i;
	int counter;
	numHist = 0;
	head->next = NULL;
	tail->next = NULL;
	head->prev = NULL;
	tail->prev = NULL;
	head->args = NULL;
	tail->args = NULL;
	head->command = NULL;
	tail->command = NULL;
	head->next = NULL;
	tail->next = NULL;
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
		int flag = checkHist(args[0]);
		if(flag == 1) {
			histCommand* toBeCalled = findLastN(1);
			if(toBeCalled != NULL) {
				histCommand* newHist = createNode(toBeCalled->command, toBeCalled->args, toBeCalled->numArg);
				addToTail(newHist);
				callChild(newHist->numArg, newHist->args);
			}
			free(args);
			free(commandLine);
		}
		else if(flag == 2) {
			histCommand* toBeCalled = findLastN(atoi(args[0]+2));
			if(toBeCalled != NULL) {
				histCommand* newHist = createNode(toBeCalled->command, toBeCalled->args, toBeCalled->numArg);
				addToTail(newHist);
				callChild(newHist->numArg, newHist->args);
			}
			free(args);
			free(commandLine);
		}
		else if(flag == 3) {
			histCommand* toBeCalled = findFirstN(atoi(args[0]+1));
			if(toBeCalled != NULL) {
				histCommand* newHist = createNode(toBeCalled->command, toBeCalled->args, toBeCalled->numArg);
				addToTail(newHist);
				callChild(newHist->numArg, newHist->args);
			}
			free(args);
			free(commandLine);
		}
		else if(flag == 0) {
			histCommand* newHist = createNode(commandLine, args, numArg);
			addToTail(newHist);
			callChild(newHist->numArg, newHist->args);
		}
		else {
			free(args);
			free(commandLine);
		}
		//callChild(numArg, args);
		
		//free(args);
		//free(commandLine);
	}
	return 0;
}

void callChild(int cargc, char **argv) {
	pid_t pid;
	int status;
	if(cargc == 1 && !strcmp(argv[0], "exit")) {
		printf("Exit to main shell in a second!\n");
		//free(args);
		//free(commandLine);
		clearList();

		sleep(1);
		exit(0);
	}
	if(cargc == 1 && !strcmp(argv[0], "history"))
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
			//free(args);
			//free(commandLine);
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

histCommand* createNode(char* command, char** args, int numArg) {
	histCommand* result = malloc(sizeof(histCommand));
	result->next = NULL;
	result->prev = NULL;
	result->numArg = numArg;
	result->args = args;
	result->command = command;
	return result;
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
		printf("Sorry! No such history command!\n");
		//exit(0);
		return NULL;
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
		printf("Sorry! No such history command!\n");
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
	free(current->command);
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

void showHistory() {
	if(!head->next) {
		return;
	}
	else {
		histCommand* temp = head->next;
		while(temp->next->args) {
			printf("%s\n", temp->command);
			temp = temp->next
		}
	}
}

/* return 1 if toCheck is last
   return 2 if toCheck is last n
   return 3 if toCheck is first n
   return -1 if is invalid 
   return 0 if it's valid but not calling history

*/
int checkHist(char* toCheck) {
	if (!strcmp(toCheck, "!!")) {
		return 1;
	}
	else if (isBeginWith(toCheck , "!-")) {
		if (atoi(toCheck+2) == 0) {
			printf("Pleas type an interger right after !- .\n");
			return -1;
		}
		else {
			return 2
		}
	}
	else if(isBeginWith(toCheck , "!")) {
		if (atoi(toCheck+1) == 0) {
			printf("Pleas type an interger right after ! .\n");
			return -1;
		}
		else {
			return 3;
		}
	}
	return 0; 
}

int isBeginWith(const char * str1,char *str2)
{
  if(str1 == NULL || str2 == NULL)
    return -1;
  int len1 = strlen(str1);
  int len2 = strlen(str2);
  if((len1 < len2) || (len1 == 0 || len2 == 0))
    return -1;
  char *p = str2;
  int i = 0;
  while(*p != '\0') {
    if(*p != str1[i]) {
      return 0;
    }
    p++;
    i++;
  }
  return 1;
}








