#include <queue>
#include <string>
#include <ucontext> 
#include <stdio> 


#define QUANTA 100 // the length of a quanta is set to be 100ms
#define DEFAULT_RUNTIME 50  // the length of a default run time is set to be 50


queue<myThread> ready_queue;
queue<myThread> blocked_queue;
enum {FALSE, TRUE};
int schedule_policy;


typedef struct Threadlist{
	int id;
	int pid;
	int child_count;
	int exit_status;
	int wait_status;
	int active;
	int join;
	int join_count;
	int policy;
	int priority;
	ucontext_t context; 
} myThread;


