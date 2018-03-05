#include <queue>
#include <list>
#include <string>
#include <ucontext> 
#include <stdio> 


#define QUANTA 100 // the length of a quanta is set to be 100ms
#define DEFAULT_RUNTIME 50  // the length of a default run time is set to be 50


list<myThread> ready_queue;
list<myThread> blocked_queue;
priority_queue<thread_SJF> ready_SJF; // store the id of next ready thread with sjf policy
priority_queue<thread_PRI> ready_PRI;	// similar things

enum {FALSE, TRUE};
enum {FIRST, SECOND, THIRD}; // the enum value for each priority, 0, 1, 2 respectively
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
	int estimated_runtime;
	ucontext_t context; 
} myThread;

typedef struct
{
	int id;
	int estimated_runtime;
	friend bool operator < (thread_SJF a, thread_SJF b)  
    {  
        return a.estimated_runtime > b.estimated_runtime;  //threads with smaller estimated runtime has a higher priority 
    }	
} thread_SJF;

typedef struct
{
	int id;
	int priority;
		friend bool operator < (thread_PRI a, thread_PRI b)  
    {  
        return a.priority > b.priority;  //threads with smaller priority has a higher priority 
    }
} thread_PRI;


myThread choose_next_thread_FIFO();
myThread choose_next_thread_SJF();
myThread choose_next_thread_PRI();
myThread choose_next_thread(int policy);














