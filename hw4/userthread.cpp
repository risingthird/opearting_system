#include "userthread.h"
#include "util.h"


using namespace std;


static int initialized = FALSE;

int thread_libinit(int policy) {
	if (policy != _FIFO && policy != _PRIORITY && policy != _SJF) {
		return EXIT_WITH_ERROR;
	}

	if (initialized == TRUE) {
		return EXIT_WITH_ERROR;
	}

	initialized = TRUE;

	log_file.open ("log_file.txt", std::fstream::in | std::fstream::out | std::fstream::trunc);
	struct timeval tv;
	gettimeofday(&tv, NULL);
	start_time = tv.tv_sec*MICRO_TO_MILI + tv.tv_usec/MICRO_TO_MILI;
	sigemptyset(&thread_mask);
	sigaddset(&thread_mask, SIGALRM);
	scheduler_stack = malloc(STACKSIZE);
	schedule_policy = policy;
	if (scheduler_stack == NULL) {
		//printf("from line 10\n");
		return EXIT_WITH_ERROR;
	}
	current_active = NULL;
	if (schedule_policy == _FIFO) {
		ready_FIFO = queue<int> ();
		thread_list_head = list<myThread*> ();
		thread_count = FIRST_THREAD;

	}
	else if (schedule_policy == _SJF) {
		thread_list_head = list<myThread*> ();
		ready_SJF = priority_queue<thread_PRI_SJF_FIFO*, vector<thread_PRI_SJF_FIFO*>, compare> ();
		thread_count = FIRST_THREAD;
	}
	else if (schedule_policy == _PRIORITY) {
		thread_list_head = list<myThread*> ();
		ready_queue_first = list<int> ();
		ready_queue_second = list<int> (); 
		ready_queue_third = list<int> (); 
		thread_count = FIRST_THREAD; 
	}
	else {
		return EXIT_WITH_ERROR;
	}

	if (schedule_policy == _PRIORITY) {
		signal(SIGALRM, sigalarm_handler);
		priority_timer.it_value.tv_sec = 0;    
		priority_timer.it_value.tv_usec = QUANTA * MICRO_TO_MILI;  
		priority_timer.it_interval.tv_sec = 0;  
		priority_timer.it_interval.tv_usec = QUANTA * MICRO_TO_MILI; 
		if (setitimer(ITIMER_REAL, &priority_timer, NULL) < 0) {
			return EXIT_WITH_ERROR;	
		}
	}
	getcontext(&scheduler_context);
	scheduler_context.uc_link = 0;
	scheduler_context.uc_stack.ss_sp = scheduler_stack;
	scheduler_context.uc_stack.ss_size = STACKSIZE;
	scheduler_context.uc_stack.ss_flags = 0;
	makecontext(&scheduler_context, my_scheduler, 0);

	main_stack = malloc(STACKSIZE);	
	if (main_stack == NULL) {
		//printf("from line 25\n");
		return EXIT_WITH_ERROR;
	}
	
	getcontext(&main_context);
	main_context.uc_stack.ss_sp = main_stack;
	main_context.uc_stack.ss_size = STACKSIZE;

	//printf("I died here\n");
	return EXIT_SUCCESS;
}

int thread_libterminate() {
	if(initialized == FALSE) {
		return EXIT_WITH_ERROR;
	}
	log_file.close();
	if (scheduler_stack != NULL) {
		free(scheduler_stack);
		scheduler_stack = NULL;
	}
	if (main_stack != NULL) {
		free(main_stack);
		main_stack = NULL;
	}
	return util_terminate();
}

int thread_create(void (*func)(void *), void *arg, int priority) {
	
	if (priority != FIRST-1 && priority != SECOND-1 && priority != THIRD-1) {
		return EXIT_WITH_ERROR;
	}
	
	if (func == NULL) {
		return EXIT_WITH_ERROR;
	}

	if (initialized == FALSE) {
		return EXIT_WITH_ERROR;
	}

	void* stack = malloc(STACKSIZE);
	ucontext_t current_context;
	getcontext(&current_context);
	thread_count++;
	current_context.uc_link = 0;
	current_context.uc_stack.ss_sp = stack;
	current_context.uc_stack.ss_size = STACKSIZE;
	
	if(current_context.uc_stack.ss_sp == NULL) {
		//printf("from line 71\n");
		return EXIT_WITH_ERROR;
	}

	current_context.uc_stack.ss_flags = 0;
	makecontext(&current_context, (void(*)())thread_wrapper, 2, func, arg);  // wrap the called function
	myThread* current_thread = new myThread();
	current_thread->tid = thread_count;
	current_thread->yield_count = 0;
	current_thread->start_time = 0;
	current_thread->end_time = 0;
	current_thread->time_first = 0;
	current_thread->time_second = 0;
	current_thread->time_third = 0;
	current_thread->status = CREATED;
	current_thread->policy = schedule_policy;
	current_thread->priority = priority;
	current_thread->estimated_runtime = DEFAULT_RUNTIME;
	current_thread->wait_tid = NOT_FOUND;
	current_thread->stack = stack;
	current_thread->context = current_context;
	current_thread->suspended_queue = queue<int> ();
	thread_list_head.push_back(current_thread);
	if (schedule_policy == _FIFO) {
		ready_FIFO.push(thread_count);
	}
	else if (schedule_policy == _SJF) {
		thread_PRI_SJF_FIFO* toAdd = new thread_PRI_SJF_FIFO();
		toAdd->id = thread_count;
		toAdd->priority = QUANTA / 2;
		ready_SJF.push(toAdd);
	}
	else if (schedule_policy == _PRIORITY) {
		if (priority == FIRST-1) {
			//printf("Priorit is %d!!!!!!!!!!!!!!!!!!!!!!!     Thread id is %d\n", priority, thread_count);
			ready_queue_first.push_back(thread_count);
		}
		else if (priority == SECOND-1) {
			//printf("Priorit is %d!!!!!!!!!!!!!!!!!!!!!!!     Thread id is %d\n", priority, thread_count);
			ready_queue_second.push_back(thread_count);
		}
		else if (priority == THIRD-1) {
			//printf("Priorit is %d!!!!!!!!!!!!!!!!!!!!!!!     Thread id is %d\n", priority, thread_count);
			ready_queue_third.push_back(thread_count);
		}
		else {
			//printf("from line 94\n");
			return EXIT_WITH_ERROR;
		}
	}
	else {
		//printf("from line 99\n");
		return EXIT_WITH_ERROR; // error message skipped
	}

	struct timeval current_time;
	gettimeofday(&current_time, NULL);
	log_file << "[" <<(current_time.tv_sec*MICRO_TO_MILI + current_time.tv_usec/MICRO_TO_MILI) -  start_time << "]" <<" \t " << "CREATED" << " \t " << current_thread->tid << " \t " << current_thread->priority << endl;
	return thread_count;

}

int thread_yield() {
	if (initialized == FALSE) {
	 	return EXIT_WITH_ERROR;
	}

	if (schedule_policy == _PRIORITY) {
		sigprocmask(SIG_BLOCK, &thread_mask, NULL);
	}

	

	myThread* current_thread;
	ucontext_t save_context;
	current_thread = current_active;
	//getcontext(&save_context);
	if (current_thread != NULL) {
		current_thread->status = YIELD;
		//printf("Thread %d is calling yield\n", current_thread->tid);
		//current_thread->context = save_context;
		if (schedule_policy == _PRIORITY) {
			sigprocmask(SIG_UNBLOCK, &thread_mask, NULL);
		}
		makecontext(&scheduler_context, my_scheduler, 0);
		struct timeval current_time;
		gettimeofday(&current_time, NULL);
		log_file << "[" <<(current_time.tv_sec*MICRO_TO_MILI + current_time.tv_usec/MICRO_TO_MILI) -  start_time << "]" << " \t " << "STOPPED" << " \t " << current_thread->tid << " \t " << current_thread->priority << endl;
		swapcontext(&current_thread->context, &scheduler_context);
	}
	else{
		if (schedule_policy == _PRIORITY) {
			sigprocmask(SIG_UNBLOCK, &thread_mask, NULL);
		}
		makecontext(&scheduler_context, my_scheduler, 0);
		setcontext(&scheduler_context);
	}
	return EXIT_SUCCESS;
}

int thread_join(int tid) {
	if (initialized == FALSE) {
		return EXIT_WITH_ERROR;
	}

	if (schedule_policy == _PRIORITY) {
		sigprocmask(SIG_BLOCK, &thread_mask, NULL);
	}

	

	myThread* current_thread;
	current_thread = current_active;
	ucontext_t save_context;
	myThread* toWait = find_by_tid(tid);
	if (toWait == NULL) {
		//printf("thread_count is %d\n", thread_count);
		//printf("from line 130\n");
		return EXIT_WITH_ERROR;
	}
	//getcontext(&save_context);
	if (current_active != NULL) {
		current_active->status = STOPPED;
		//current_active->context = save_context;
		current_active->wait_tid = tid;
		makecontext(&scheduler_context, my_scheduler, 0);
		struct timeval current_time;
		gettimeofday(&current_time, NULL);
		log_file << "[" <<(current_time.tv_sec*MICRO_TO_MILI + current_time.tv_usec/MICRO_TO_MILI) -  start_time << "]" << " \t " << "STOPPED" << " \t " << current_thread->tid << " \t " << current_thread->priority << endl;
		if (schedule_policy == _PRIORITY) {
			sigprocmask(SIG_UNBLOCK, &thread_mask, NULL);
		}
		//printf("I am with id %d\n", current_active->tid);
		swapcontext(&current_active->context, &scheduler_context);
	}
	else {
		if (schedule_policy == _PRIORITY) {
			sigprocmask(SIG_UNBLOCK, &thread_mask, NULL);
		}
		makecontext(&scheduler_context, my_scheduler, 0);
		setcontext(&scheduler_context);
	}

	return EXIT_SUCCESS;
}






void clear_up(list<myThread*> *ll) {
	//printf("I am exiting from 180\n");
	while(!ll->empty()) {
		if(ll->front() != NULL) {
			if (ll->front()->stack != NULL) {
				free(ll->front()->stack);
				ll->front()->stack = NULL;
			}			
			delete(ll->front());
			ll->front() = NULL;
		}
		ll->pop_front();
	}
	ll->clear();
}

void clear_up_PRIqueue(list<int> *ll) {
	//printf("I am exiting from 180\n");
	while(!ll->empty()) {
		ll->pop_front();
	}
	//ll->clear();
}

void clear_up_FIFOqueue(queue<int> *ll) {
	while(!ll->empty()) {
		ll->pop();
	}
}

void clear_up_SJFqueue(priority_queue<thread_PRI_SJF_FIFO*, vector<thread_PRI_SJF_FIFO*>, compare> *ll) {
	while(!ll->empty()) {
		if(ll->top() != NULL) {
			delete(ll->top());
		}
		ll->pop();
	}
}

int util_init() {
	//schedule_policy = policy;
	current_active = NULL;
	if (schedule_policy == _FIFO) {
		ready_FIFO = queue<int> ();
		thread_list_head = list<myThread*> ();
		thread_count = FIRST_THREAD;

	}
	else if (schedule_policy == _SJF) {
		thread_list_head = list<myThread*> ();
		ready_SJF = priority_queue<thread_PRI_SJF_FIFO*, vector<thread_PRI_SJF_FIFO*>, compare> ();
		thread_count = FIRST_THREAD;
	}
	else if (schedule_policy == _PRIORITY) {
		thread_list_head = list<myThread*> ();
		ready_queue_first = list<int> ();
		ready_queue_second = list<int> (); 
		ready_queue_third = list<int> (); 
		thread_count = FIRST_THREAD; 
	}
	else {
		//printf("from line 213, current policy %d\n", schedule_policy);
		return EXIT_WITH_ERROR;
	}
	return EXIT_SUCCESS;
}

int util_terminate() {
	if (initialized == FALSE) {
		return EXIT_WITH_ERROR;
	}
	if (schedule_policy == _FIFO) {
		clear_up(&thread_list_head);
		clear_up_FIFOqueue(&ready_FIFO);
	}
	else if (schedule_policy == _SJF) {
		clear_up(&thread_list_head);
		clear_up_SJFqueue(&ready_SJF);
	}
	else if (schedule_policy == _PRIORITY) {
		clear_up(&thread_list_head);
		clear_up_PRIqueue(&ready_queue_first);
		clear_up_PRIqueue(&ready_queue_second);
		clear_up_PRIqueue(&ready_queue_third);
	}
	else {
		//printf("from line 235\n");
		return EXIT_WITH_ERROR;
	}
	initialized = FALSE;
	return EXIT_SUCCESS;

}

int find_active() {
	list<myThread*> :: iterator it;
	for(it = thread_list_head.begin(); it != thread_list_head.end(); ++it) {
		if ((*it)->active == TRUE) {
			return (*it)->tid;
		}
	}
	return NOT_FOUND;
}

myThread* find_by_tid(int tid) {
	list<myThread*>::iterator it;
	for(it = thread_list_head.begin(); it != thread_list_head.end(); ++it) {
		if ((*it)->tid == tid) {
			return *it;
		}
	}
	return NULL;
}

int choose_next_thread_FIFO() {
	int toReturn = NOT_FOUND;
	if (ready_FIFO.empty()) {
		return NOT_FOUND;
	}
	else {
		toReturn = ready_FIFO.front();
		ready_FIFO.pop();
	}
	return toReturn;
}

thread_PRI_SJF_FIFO* choose_next_thread_SJF() {
	thread_PRI_SJF_FIFO* toReturn = NULL;
	if (ready_SJF.empty()) {
		return NULL;
	}
	else {
		toReturn = ready_SJF.top();
		ready_SJF.pop();
		//printf("The ready queue has %d threads\n", ready_SJF.size());
	}
	return toReturn;	
}

int choose_next_thread_PRI() {
	int toReturn = NOT_FOUND;
	struct timeval tv;
 	gettimeofday(&tv, NULL);
 	unsigned int seed = tv.tv_usec;
	srand(seed);
	int lucky = rand() % 19;
	if (lucky < 4) {
		if (ready_queue_third.empty()) {
			if (ready_queue_first.empty()) {
				if (ready_queue_second.empty()) {
					return NOT_FOUND;
				}
				else {
					toReturn = ready_queue_second.front();
					ready_queue_second.pop_front();
				}
			}
			else {
				toReturn = ready_queue_first.front();
				ready_queue_first.pop_front();
			}
		}
		else {
			toReturn = ready_queue_third.front();
			ready_queue_third.pop_front();
		}		
	}
	else if (lucky < 10) {
		if (ready_queue_second.empty()) {
			if (ready_queue_first.empty()) {
				if (ready_queue_third.empty()) {
					return NOT_FOUND;
				}
				else {
					toReturn = ready_queue_third.front();
					ready_queue_third.pop_front();
				}
			}
			else {
				toReturn = ready_queue_first.front();
				ready_queue_first.pop_front();
			}
		}
		else {
			toReturn = ready_queue_second.front();
			ready_queue_second.pop_front();
		}		
	}
	else {
		if (ready_queue_first.empty()) {
			if (ready_queue_second.empty()) {
				if (ready_queue_third.empty()) {
					return NOT_FOUND;
				}
				else {
					toReturn = ready_queue_third.front();
					ready_queue_third.pop_front();
				}
			}
			else {
				toReturn = ready_queue_second.front();
				ready_queue_second.pop_front();
			}
		}
		else {
			toReturn = ready_queue_first.front();
			ready_queue_first.pop_front();
		}			
	}
	//printf("The chosen one is %d, with a lucky number of %d\n", toReturn, lucky);
	return toReturn;
}

int set_start_time(myThread* a_thread) {
	struct timeval tv;
 	gettimeofday(&tv,NULL);
 	a_thread->start_time = tv.tv_sec*MICRO_TO_MILI + tv.tv_usec/MICRO_TO_MILI;
 	return EXIT_SUCCESS;
}

int set_end_time(myThread* a_thread) {
	struct timeval tv;
 	gettimeofday(&tv,NULL);
 	a_thread->end_time = tv.tv_sec*MICRO_TO_MILI + tv.tv_usec/MICRO_TO_MILI;
 	//printf("END TIME IS %ld\n", a_thread->end_time);
 	return EXIT_SUCCESS;
}

int set_estimated_time(myThread* a_thread) {
	int count = (a_thread->yield_count) % 3;
	if (count == FIRST) {
		a_thread->time_first = a_thread->end_time - a_thread->start_time;
	}
	else if (count == SECOND) {
		a_thread->time_second = a_thread->end_time - a_thread->start_time;
	}
	else {
		a_thread->time_third = a_thread->end_time - a_thread->start_time;
	}

	if (a_thread->yield_count > 2) {
		a_thread->estimated_runtime = (a_thread->time_first + a_thread->time_second + a_thread->time_third) / 3;
		//printf("We are estimating thread %d\n", a_thread->tid);
	}

	return EXIT_SUCCESS;
}

void thread_wrapper(void (*func)(void *), void *arg) {
	func(arg);
	printf("I am here\n");
	if (current_active != NULL) {
		//printf("FINISHED %d\n", current_active->tid);
		current_active->status = FINISHED;
		struct timeval current_time;
		gettimeofday(&current_time, NULL);
		log_file << "[" <<(current_time.tv_sec*MICRO_TO_MILI + current_time.tv_usec/MICRO_TO_MILI) -  start_time << "]" << " \t " << "FINISHED" << " \t " << current_active->tid << " \t " << current_active->priority << endl;
		makecontext(&scheduler_context, my_scheduler, 0);
		swapcontext(&current_active->context, &scheduler_context);
	}
	makecontext(&scheduler_context, my_scheduler, 0);
	setcontext(&main_context);
}

void my_scheduler() {
	int activeID;
	int nextID;
	ucontext_t save_context, new_context;
	myThread* current_thread;
	myThread* next_thread;
	myThread* wait_thread;
	current_thread = current_active;
	if (schedule_policy == _PRIORITY) {
		sigprocmask(SIG_BLOCK, &thread_mask, NULL);
	}
	// put threads in suspended queue back to ready queue

	if (current_thread != NULL) {
		activeID = current_active->tid;
		current_thread->active = FALSE;
		if (current_thread->status == FINISHED) {
			if (schedule_policy == _FIFO) {
				while (!current_thread->suspended_queue.empty()) {
					ready_FIFO.push(current_thread->suspended_queue.front());
					//printf("thread %d is clearing its suspended queue. Thread %d is now in queue\n", current_thread->tid, current_thread->suspended_queue.front());
					current_thread->suspended_queue.pop();
				}
				nextID = choose_next_thread_FIFO();
				//printf("Thread %d is to be run. Since thread %d is done", nextID, current_thread->tid);
				if (nextID == NOT_FOUND) {
					makecontext(&scheduler_context, my_scheduler, 0);
					current_active = NULL;
					swapcontext(&scheduler_context, &main_context);
				}
			}
			else if (schedule_policy == _SJF) {
				//log_file << "[ticks]" << " \t " << "FINISHED" << " \t " << current_thread->tid << " \t " << current_thread->priority << endl;
				//printf("I am fucking here with current tid of %d\n", current_active->tid);
				while (!current_thread->suspended_queue.empty()) {
					thread_PRI_SJF_FIFO* temp = new thread_PRI_SJF_FIFO();
					temp->id = current_thread->suspended_queue.front();
					myThread* temp1 = find_by_tid(current_thread->suspended_queue.front());
					temp->priority = temp1->estimated_runtime;
					ready_SJF.push(temp);
					current_thread->suspended_queue.pop();
				}

				thread_PRI_SJF_FIFO* temp3 = NULL;
				temp3 = choose_next_thread_SJF();
				if (temp3 == NULL) {
					makecontext(&scheduler_context, my_scheduler, 0);
					//printf("I am fucking here with current tid of %d\n", current_active->tid);
					current_active = NULL;
					
					swapcontext(&scheduler_context, &main_context);
				}

				nextID = temp3->id;
				//printf("The next to arrive is %d\n", nextID);
				delete(temp3);
			}
			else if (schedule_policy == _PRIORITY) {
				while (!current_thread->suspended_queue.empty()) {
					myThread* temp1 = find_by_tid(current_thread->suspended_queue.front());
					if (temp1->priority == FIRST-1) {
						ready_queue_first.push_back(current_thread->suspended_queue.front());
					}
					else if (temp1->priority == SECOND-1) {
						ready_queue_second.push_back(current_thread->suspended_queue.front());
					}
					else {
						ready_queue_third.push_back(current_thread->suspended_queue.front());
					}
					current_thread->suspended_queue.pop();
				}
				nextID = choose_next_thread_PRI();
				if (nextID == NOT_FOUND) {
					makecontext(&scheduler_context, my_scheduler, 0);
					sigprocmask(SIG_UNBLOCK, &thread_mask, NULL);
					//printf("I am going to return from 504 to main_context\n");
					current_active = NULL;
					swapcontext(&scheduler_context, &main_context);
				}
			}

		}
		else if (current_thread->status == YIELD){
			if (schedule_policy == _FIFO) {
				ready_FIFO.push(activeID);
				nextID = choose_next_thread_FIFO();
				if (nextID == NOT_FOUND) {
					makecontext(&scheduler_context, my_scheduler, 0);
					current_active = NULL;
					swapcontext(&scheduler_context, &main_context);
				}
			}
			else if (schedule_policy == _SJF) {
				set_end_time(current_thread);
				set_estimated_time(current_thread);
				current_thread->yield_count++;
				thread_PRI_SJF_FIFO* temp = NULL;
				thread_PRI_SJF_FIFO* temp2 = new thread_PRI_SJF_FIFO();
				temp = choose_next_thread_SJF();
				if (temp == NULL) {
					delete(temp2);
					makecontext(&scheduler_context, my_scheduler, 0);
					current_active = NULL;
					swapcontext(&scheduler_context, &main_context);
				}
				nextID = temp->id;
				delete(temp);
				temp2->id = current_thread->tid;
				temp2->priority = current_thread->estimated_runtime;
				//printf("We have run time of %lf for thread %d\n", current_thread->estimated_runtime, current_thread->tid);
				ready_SJF.push(temp2);
			}
			else {
				/*To do here*/
				if (current_thread->priority == FIRST-1) {
					ready_queue_first.push_back(current_thread->tid);
				}
				else if (current_thread->priority == SECOND-1) {
					ready_queue_second.push_back(current_thread->tid);
				}
				else if (current_thread->priority == THIRD-1) {
					ready_queue_third.push_back(current_thread->tid);
				}
				else {
					makecontext(&scheduler_context, my_scheduler, 0);
					current_active = NULL;
					swapcontext(&scheduler_context, &main_context);
				}
				nextID = choose_next_thread_PRI();
				if (nextID == NOT_FOUND) {
					makecontext(&scheduler_context, my_scheduler, 0);
					sigprocmask(SIG_UNBLOCK, &thread_mask, NULL);
					//printf("I am going to return from 556 to main_context\n");
					current_active = NULL;
					swapcontext(&scheduler_context, &main_context);
				}
			}
		}
		else if (current_thread->status == STOPPED){
			if (schedule_policy == _FIFO) {
				nextID = choose_next_thread_FIFO();
				if (nextID == NOT_FOUND) {
					makecontext(&scheduler_context, my_scheduler, 0);
					current_active = NULL;
					swapcontext(&scheduler_context, &main_context);
				}
				wait_thread = find_by_tid(current_thread->wait_tid);
				wait_thread->suspended_queue.push(activeID);
			}
			else if (schedule_policy == _SJF) {
				set_end_time(current_thread);
				set_estimated_time(current_thread);
				current_thread->yield_count++;
				thread_PRI_SJF_FIFO* temp = NULL;
				temp = choose_next_thread_SJF();
				if (temp == NULL) {
					makecontext(&scheduler_context, my_scheduler, 0);
					current_active = NULL;
					swapcontext(&scheduler_context, &main_context);
				}
				nextID = temp->id;
				delete(temp);
				wait_thread = find_by_tid(current_thread->wait_tid);
				wait_thread->suspended_queue.push(activeID);
			}
			else {
				/*To do here*/
				nextID = choose_next_thread_PRI();
				if (nextID == NOT_FOUND) {
					makecontext(&scheduler_context, my_scheduler, 0);
					sigprocmask(SIG_UNBLOCK, &thread_mask, NULL);
					current_active = NULL;
					//printf("I am going to return from 591 to main_context\n");
					swapcontext(&scheduler_context, &main_context);
				}
				wait_thread = find_by_tid(current_thread->wait_tid);
				wait_thread->suspended_queue.push(activeID);								
			}	
		}
	}
	else {
		if (schedule_policy == _FIFO) {
			nextID = choose_next_thread_FIFO();
			if (nextID == NOT_FOUND) {
				makecontext(&scheduler_context, my_scheduler, 0);
				current_active = NULL;
				swapcontext(&scheduler_context, &main_context);
			}
		}
		else if (schedule_policy == _SJF) {
			thread_PRI_SJF_FIFO* temp = NULL;
			temp = choose_next_thread_SJF();
			if (temp == NULL) {
				makecontext(&scheduler_context, my_scheduler, 0);
				current_active = NULL;
				swapcontext(&scheduler_context, &main_context);
			}
			nextID = temp->id;
			delete(temp);
		}
		else {
			nextID = choose_next_thread_PRI();
			if (nextID == NOT_FOUND) {
				makecontext(&scheduler_context, my_scheduler, 0);
				sigprocmask(SIG_UNBLOCK, &thread_mask, NULL);
				current_active = NULL;
				//printf("I am going to return from 621 to main_context\n");
				swapcontext(&scheduler_context, &main_context);
			}
		}
	}
	next_thread = find_by_tid(nextID);
	next_thread->active = TRUE;
	next_thread->status = SCHEDULED;
	struct timeval current_time;
	gettimeofday(&current_time, NULL);
	log_file << "[" <<(current_time.tv_sec*MICRO_TO_MILI + current_time.tv_usec/MICRO_TO_MILI) -  start_time << "]" << " \t " << "SCHEDULED" << " \t " << next_thread->tid << " \t " << next_thread->priority << endl;
	current_active = next_thread;
	//printf("%d\n", current_active->tid);
	set_start_time(next_thread);

	if (schedule_policy == _PRIORITY) {
		priority_timer.it_value.tv_sec = 0;    
		priority_timer.it_value.tv_usec = QUANTA * MICRO_TO_MILI;  
		priority_timer.it_interval.tv_sec = 0;  
		priority_timer.it_interval.tv_usec = QUANTA * MICRO_TO_MILI; 
		if (setitimer(ITIMER_REAL, &priority_timer, NULL) < 0) {
			makecontext(&scheduler_context, my_scheduler, 0);
			current_active = NULL;
			sigprocmask(SIG_UNBLOCK, &thread_mask, NULL);
			swapcontext(&scheduler_context, &main_context);		
		}
		sigprocmask(SIG_UNBLOCK, &thread_mask, NULL);
	}

	makecontext(&scheduler_context, my_scheduler, 0);
	swapcontext(&scheduler_context, &(next_thread->context));
}

void sigalarm_handler(int sig) {
	if (schedule_policy == _PRIORITY) {
		sigprocmask(SIG_BLOCK, &thread_mask, NULL);
	}
	myThread* current_thread;
	ucontext_t save_context;
	current_thread = current_active;
	getcontext(&save_context);
	if (current_thread != NULL) {
		current_thread->status = YIELD;
		//current_thread->context = save_context;
		struct timeval current_time;
		gettimeofday(&current_time, NULL);
		log_file << "[" <<(current_time.tv_sec*MICRO_TO_MILI + current_time.tv_usec/MICRO_TO_MILI) -  start_time << "]" << " \t " << "STOPPED" << " \t " << current_thread->tid << " \t " << current_thread->priority << endl;
		makecontext(&scheduler_context, my_scheduler, 0);
		if (schedule_policy == _PRIORITY) {
			sigprocmask(SIG_UNBLOCK, &thread_mask, NULL);
		}
		swapcontext(&current_thread->context, &scheduler_context);
	}
	else{
		//log_file << "[ticks]" << " \t " << "STOPPED" << " \t " << current_thread->tid << " \t " << current_thread->priority << endl;
		makecontext(&scheduler_context, my_scheduler, 0);
		setcontext(&scheduler_context);
	}
}




































