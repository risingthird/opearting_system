#ifndef __UTIL_H_
#define __UTIL_H_

#include <queue>
#include <list>
#include <string.h>
#include <ucontext.h> 
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <unistd.h> 


#define QUANTA 0.001 // the length of a quanta is set to be 100ms
#define DEFAULT_RUNTIME 50  // the length of a default run time is set to be 50
#define FIRST_THREAD 0
#define EXIT_SUCCESS 0
#define EXIT_WITH_ERROR -1
#define NOT_FOUND -1

using namespace std;

enum {FALSE, TRUE};
enum {FIRST, SECOND, THIRD}; // the enum value for each priority, 0, 1, 2 respectively
enum {CREATED, SCHEDULED, STOPPED, FINISHED, YIELD};
enum {FIFO, SJF, PRIORITY};


typedef struct ThreadList{
	int tid;
	int status;
	int active;     // true or false
	int policy;
	int priority;
	int yield_count;
	int wait_tid;
	long time_first;
	long time_second;
	long time_third;
	double estimated_runtime;
	long start_time;
	long end_time;
	queue<int> suspended_queue;
	void* stack;
	ucontext_t context; 
} myThread;

typedef struct temp2
{
	int id;
	int priority;
		friend bool operator < (struct temp2 a, struct temp2 b)  
    {  
        return a.priority > b.priority;  //threads with smaller priority has a higher priority 
    }
} thread_PRI_SJF_FIFO;

static int thread_count;
static int schedule_policy;
static myThread* main_thread;
static myThread* current_active;
static ucontext_t scheduler_context;
static list<myThread*> thread_list_head;
//static list<myThread*> thread_list_head; // keep all created threads
static list<int> ready_queue_first;
static list<int> ready_queue_second;
static list<int> ready_queue_third;
static queue<int> ready_FIFO; 
static priority_queue<thread_PRI_SJF_FIFO*> ready_SJF; // store the id of next ready thread with sjf policy
void* scheduler_stack;





int choose_next_thread_FIFO();
thread_PRI_SJF_FIFO* choose_next_thread_SJF();
int choose_next_thread_PRI();
myThread* choose_by_tid(int tid);
myThread* find_by_tid(int tid);
myThread* choose_next_thread(int policy);
int find_active(); // return tid of current active thread
int util_init(int policy);
int util_terminate();
int set_start_time(myThread* a_thread);
int set_end_time(myThread* a_thread);
int set_estimated_time(myThread* a_thread);
void clear_up(list<myThread*> *ll);
void clear_up_PRIqueue(list<thread_PRI_SJF_FIFO*> *ll);
void clear_up_FIFOqueue(queue<thread_PRI_SJF_FIFO*> *ll);
void clear_up_SJFqueue(priority_queue<thread_PRI_SJF_FIFO*> *ll); 
void thread_wrapper(void (*func)(void *), void *arg);
void my_scheduler();


#endif












