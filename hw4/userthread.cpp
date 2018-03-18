#include "userthread.h"
#include "util.h"

int thread_libinit(int policy) {
	return util_init(policy);
}

int thread_libterminate() {
	return util_terminate();
}

int thread_create(void (*func)(void *), void *arg, int priority) {
	void* stack = malloc(STACKSIZE);
	ucontext current_context;
	getcontext(&current_context);
	
	current_context.uc_link = 0;
	current_context.uc_stack.ss_sp = stack;
	current_context.uc_stack.ss_size = STACKSIZE;
	
	if(current_context.uc_stack.ss_sp == NULL) {
		return EXIT_WITH_ERROR;
	}

	current_context.uc_stack.ss_flags = 0;
	makecontext(&current_context, thread_wrapper, 2, func, arg);  // wrap the called function
	myThread* current_thread = new myThread();
	current_thread->tid = ++thread_count;
	current_thread->yield_count = 0;
	current_thread->start_time = 0;
	current_thread->end_time = 0;
	current_thread->time_first = 0;
	current_thread->time_second = 0;
	current_thread->time_third = 0;
	current_thread->status = CREATED;
	current_thread->policy = schedule_policy;
	current_thread->priority = priority;
	current_thread->estimated_runtime = QUANTA / 2;
	current_thread->wait_tid = NOT_FOUND;
	current_thread->stack = stack;
	current_thread->context = current_context;
	current_thread->suspended_queue = queue<int> ();
	thread_list_head.push_back(current_thread);
	if (schedule_policy == FIFO) {
		thread_PRI_SJF_FIFO* toAdd = new thread_PRI_SJF_FIFO();
		toAdd->id = thread_count;
		ready_FIFO.push(toAdd);
	}
	else if (schedule_policy == SJF) {
		thread_PRI_SJF_FIFO* toAdd = new thread_PRI_SJF_FIFO();
		toAdd->id = thread_count;
		toAdd->priority = QUANTA / 2;
		ready_SJF.push(toAdd);
	}
	else if (schedule_policy == PRI) {
		if (priority == FIRST-1) {
			thread_PRI_SJF_FIFO* toAdd = new thread_PRI_SJF_FIFO();
			toAdd->id = thread_count;
			ready_queue_first.push(toAdd);
		}
		else if (priority == SECOND-1) {
			thread_PRI_SJF_FIFO* toAdd = new thread_PRI_SJF_FIFO();
			toAdd->id = thread_count;
			ready_queue_second.push(toAdd);
		}
		else if (priority == THIRD-1) {
			thread_PRI_SJF_FIFO* toAdd = new thread_PRI_SJF_FIFO();
			toAdd->id = thread_count;
			ready_queue_third.push(toAdd);
		}
		else {
			return EXIT_WITH_ERROR;
		}
	}
	else {
		return EXIT_WITH_ERROR; // error message skipped
	}
	return thread_count;

}

int thread_yield() {
	int activeID;
	int nextID;
	ucontext_t save_context, new_context;
	myThread* current_thread;
	myThread* next_thread;
	activeID = current_active->tid;
	current_thread = current_active;
	current_thread->active = FALSE;
	if (schedule_policy == FIFO) {
		ready_FIFO.push(activeID);
		nextID = choose_next_thread_FIFO();
		if (nextID == NOT_FOUND) {
			return EXIT_WITH_ERROR;
		}
		next_thread = find_by_tid(nextID);
		next_thread->active = TRUE;
		current_active = next_thread;
		getcontext(&save_context);
		current_thread->context = save_context;
		swapcontext(&current_thread->context, &next_thread->context);
	}
	else if (schedule_policy == SJF) {
		set_end_time(current_thread);
		set_estimated_time(current_thread);
		current_thread->yield_count++;
		thread_PRI_SJF_FIFO* temp = NULL;
		thread_PRI_SJF_FIFO* temp2 = new thread_PRI_SJF_FIFO();
		temp = choose_next_thread_SJF();
		if (temp == NULL) {
			return EXIT_WITH_ERROR;
		}
		next_thread = find_by_tid(temp->id);
		set_start_time(next_thread);
		delete(temp);
		temp2->id = current_thread->tid;
		temp2->priority = current_thread->estimated_runtime;
		ready_SJF.push(temp2);
		next_thread->active = TRUE;
		getcontext(&save_context);
		current_thread->context = save_context;
		current_active = next_thread;
		swapcontext(&current_thread->context, &next_thread->context);

	}
	else if (schedule_policy == PRI){

	}
}