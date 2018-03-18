#include "userthread.h"
#include "util.h"

int thread_libinit(int policy) {
	scheduler_stack = malloc(STACKSIZE);
	if (scheduler_stack == NULL) {
		return EXIT_WITH_ERROR;
	}
	get(&scheduler_context);
	scheduler_stack.uc_link = 0;
	scheduler_stack.uc_stack.ss_sp = scheduler_stack;
	scheduler_stack.uc_stack.ss_size = STACKSIZE;
	scheduler_stack.uc_stack.ss_flags = 0;
	makecontext(&scheduler_context, my_scheduler, 0);
	return util_init(policy);
}

int thread_libterminate() {
	free(scheduler_stack);
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
	myThread* current_thread;
	current_thread = current_active;
	current_thread->status = YIELD;
	ucontext save_context;
	getcontext(&save_context);
	current_thread->context = save_context;
	swapcontext(&save_context, &scheduler_context);
	return EXIT_SUCCESS;
}

int thread_join(int tid) {
	myThread* current_thread;
	current_thread = current_active;
	current_thread->status = STOPPED;
	current_thread->wait_tid = tid;
	ucontext save_context;
	getcontext(&save_context);
	current_thread->context = save_context;
	swapcontext(&save_context, &scheduler_context);
	return EXIT_SUCCESS;
}





