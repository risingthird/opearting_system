#include "util.h"


using namespace std;





void clear_up(list<myThread*> *ll) {
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
	while(!ll->empty()) {
		ll->pop_front();
	}
	ll->clear();
}

void clear_up_FIFOqueue(queue<int> *ll) {
	while(!ll->empty()) {
		ll->pop();
	}
}

void clear_up_SJFqueue(priority_queue<thread_PRI_SJF_FIFO*> *ll) {
	while(!ll->empty()) {
		if(ll->top() != NULL) {
			delete(ll->top());
		}
		ll->pop();
	}
}

int util_init(int policy) {
	schedule_policy = policy;
	current_active = NULL;
	if (schedule_policy == FIFO) {
		ready_FIFO = queue<int> ();
		thread_list_head = list<myThread*> ();
		thread_count = FIRST_THREAD;

	}
	else if (schedule_policy == SJF) {
		thread_list_head = list<myThread*> ();
		ready_SJF = priority_queue<thread_PRI_SJF_FIFO*> ();
		thread_count = FIRST_THREAD;
	}
	else if (schedule_policy == PRIORITY) {
		thread_list_head = list<myThread*> ();
		ready_queue_first = list<int> ();
		ready_queue_second = list<int> (); 
		ready_queue_third = list<int> (); 
		thread_count = FIRST_THREAD; 
	}
	else {
		printf("Wrong policy number\n");
		return EXIT_WITH_ERROR;
	}
	return EXIT_SUCCESS;
}

int util_terminate() {
	if (schedule_policy == FIFO) {
		clear_up(&thread_list_head);
		clear_up_FIFOqueue(&ready_FIFO);
	}
	else if (schedule_policy == SJF) {
		clear_up(&thread_list_head);
		clear_up_SJFqueue(&ready_SJF);
	}
	else if (schedule_policy == PRIORITY) {
		clear_up(&thread_list_head);
		clear_up_PRIqueue(&ready_queue_first);
		clear_up_PRIqueue(&ready_queue_second);
		clear_up_PRIqueue(&ready_queue_third);
	}
	else {
		printf("Wrong policy number\n");
		return EXIT_WITH_ERROR;
	}
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

// myThread* choose_by_tid(int tid) {
// 	myThread* toReturn = NULL;
// 	list<myThread*> :: iterator it;
// 	for(it = thread_list_head.begin(); it != thread_list_head.end(); ++it) {
// 		if (*it->tid == tid) {
// 			toReturn = *it;
// 			break;
// 		}
// 	}
	
// 	if (toReturn == NULL) {
// 		printf("Bad thread id\n");
// 		return EXIT_WITH_ERROR;
// 	}

// 	if (schedule_policy == FIFO) {
// 		queue<thread_PRI_SJF_FIFO*> temp = queue<thread_PRI_SJF_FIFO*> ();
// 		thread_PRI_SJF_FIFO* temp1 = NULL;
// 		while(!ready_FIFO.empty()) {
// 			temp1 = ready_FIFO.front();
// 			if (temp1->id == tid) {
// 				delete(temp1);
// 				ready_FIFO.pop();
// 				break;
// 			}
// 			else {
// 				ready_FIFO.pop();
// 				temp.push(temp1);
// 			}
// 		}

// 		// set the priority queue to its original version but without the selected thread
// 		while(!temp.empty()) {
// 			temp1 = temp.top();
// 			temp.pop();
// 			ready_FIFO.push(temp1);
// 		}
// 	}
// 	else if (schedule_policy == SJF) {
// 		priority_queue<thread_PRI_SJF_FIFO*> temp = priority_queue<thread_PRI_SJF_FIFO*> ();
// 		thread_PRI_SJF_FIFO* temp1 = NULL;
// 		while(!ready_SJF.empty()) {
// 			temp1 = ready_SJF.top();
// 			if (temp1->id == tid) {
// 				delete(temp1);
// 				ready_SJF.pop();
// 				break;
// 			}
// 			else {
// 				ready_SJF.pop();
// 				temp.push(temp1);
// 			}
// 		}

// 		// set the priority queue to its original version but without the selected thread
// 		while(!temp.empty()) {
// 			temp1 = temp.top();
// 			temp.pop();
// 			ready_SJF.push(temp1);
// 		}
// 	}
// 	// delete the selected thread
// 	else if (schedule_policy == PRIORITY) {
// 		if (toReturn->priority == FIRST-1) {
// 			list<thread_PRI_SJF_FIFO*> :: iterator itt;
// 			for(itt = ready_queue_first.begin(); itt != ready_queue_first.end();) {
// 				if (*itt->id == tid) {
// 					delete(*itt);
// 					itt = ready_queue_first.erase(itt);
// 					break;
// 				}
// 				else {
// 					++itt;
// 				}
// 			}
// 		}
// 		else if (toReturn->priority == SECOND-1) {
// 			list<thread_PRI_SJF_FIFO*> :: iterator itt;
// 			for(itt = ready_queue_second.begin(); itt != ready_queue_second.end();) {
// 				if (*itt->id == tid) {
// 					delete(*itt);
// 					itt = ready_queue_second.erase(itt);
// 					break;
// 				}
// 				else {
// 					++itt;
// 				}
// 			}
// 		}
// 		else if (toReturn->priority == THIRD-1) {
// 			list<thread_PRI_SJF_FIFO*> :: iterator itt;
// 			for(itt = ready_queue_third.begin(); itt != ready_queue_third.end();) {
// 				if (*itt->id == tid) {
// 					delete(*itt);
// 					itt = ready_queue_third.erase(itt);
// 					break;
// 				}
// 				else {
// 					++itt;
// 				}
// 			}
// 		}
// 		else {
// 			printf("Wrong priority number\n");
// 			exit(EXIT_WITH_ERROR);
// 		}
// 	}
// 	else {
// 		printf("Wrong policy number\n");
// 		return NULL;
// 	}
// 	return toReturn;
// }

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
	}
	return toReturn;	
}

int choose_next_thread_PRI() {
	int toReturn = NOT_FOUND;
	srand(time(NULL));
	int lucky = rand() % 19;
	if (lucky < 4) {
		if (ready_queue_third.empty()) {
			if (ready_queue_second.empty()) {
				if (ready_queue_first.empty()) {
					return NOT_FOUND;
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
	return toReturn;
}

int set_start_time(myThread* a_thread) {
	struct timeval tv;
 	gettimeofday(&tv,NULL);
 	a_thread->start_time = tv.tv_sec*1000 + tv.tv_usec/1000;
 	return EXIT_SUCCESS;
}

int set_end_time(myThread* a_thread) {
	struct timeval tv;
 	gettimeofday(&tv,NULL);
 	a_thread->end_time = tv.tv_sec*1000 + tv.tv_usec/1000;
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

	if (a_thread->time_first != 0 && a_thread->time_second != 0 && a_thread->time_third != 0) {
		a_thread->estimated_runtime = (a_thread->time_first + a_thread->time_second + a_thread->time_third) / 3;
	}

	return EXIT_SUCCESS;
}

void thread_wrapper(void (*func)(void *), void *arg) {
	func(arg);
	current_active->status = FINISHED;
	makecontext(&scheduler_context, my_scheduler, 0);
	swapcontext(&current_active->context, &scheduler_context);
}

void my_scheduler() {
	int activeID;
	int nextID;
	ucontext_t save_context, new_context;
	myThread* current_thread;
	myThread* next_thread;
	myThread* wait_thread;
	current_thread = current_active;
	// put threads in suspended queue back to ready queue
	if (current_thread != NULL) {
		activeID = current_active->tid;
		current_thread->active = FALSE;
		if (current_thread->status == FINISHED) {
			if (schedule_policy == FIFO) {
				while (!current_thread->suspended_queue.empty()) {
					ready_FIFO.push(current_thread->suspended_queue.front());
					current_thread->suspended_queue.pop();
				}
				nextID = choose_next_thread_FIFO();
				if (nextID == NOT_FOUND) {
					makecontext(&scheduler_context, my_scheduler, 0);
					swapcontext(&scheduler_context, &(main_thread->context));
				}
			}
			else if (schedule_policy == SJF) {
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
					swapcontext(&scheduler_context, &(main_thread->context));
				}
				nextID = temp3->id;
				delete(temp3);
			}
			else if (schedule_policy == PRIORITY) {
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
					swapcontext(&scheduler_context, &(main_thread->context));
				}
			}
		}
		else if (current_thread->status == YIELD){
			if (schedule_policy == FIFO) {
				ready_FIFO.push(activeID);
				nextID = choose_next_thread_FIFO();
				if (nextID == NOT_FOUND) {
					makecontext(&scheduler_context, my_scheduler, 0);
					swapcontext(&scheduler_context, &(main_thread->context));
				}
			}
			else if (schedule_policy == SJF) {
				set_end_time(current_thread);
				set_estimated_time(current_thread);
				current_thread->yield_count++;
				thread_PRI_SJF_FIFO* temp = NULL;
				thread_PRI_SJF_FIFO* temp2 = new thread_PRI_SJF_FIFO();
				temp = choose_next_thread_SJF();
				if (temp == NULL) {
					delete(temp2);
					makecontext(&scheduler_context, my_scheduler, 0);
					swapcontext(&scheduler_context, &(main_thread->context));
				}
				delete(temp);
				temp2->id = current_thread->tid;
				temp2->priority = current_thread->estimated_runtime;
				ready_SJF.push(temp2);
			}
			else {
				/*To do here*/
			}
		}
		else if (current_thread->status == STOPPED){
			if (schedule_policy == FIFO) {
				nextID = choose_next_thread_FIFO();
				if (nextID == NOT_FOUND) {
					makecontext(&scheduler_context, my_scheduler, 0);
					swapcontext(&scheduler_context, &(main_thread->context));
				}
				wait_thread = find_by_tid(current_thread->wait_tid);
				wait_thread->suspended_queue.push(activeID);
			}
			else if (schedule_policy == SJF) {
				set_end_time(current_thread);
				set_estimated_time(current_thread);
				current_thread->yield_count++;
				thread_PRI_SJF_FIFO* temp = NULL;
				temp = choose_next_thread_SJF();
				if (temp == NULL) {
					makecontext(&scheduler_context, my_scheduler, 0);
					swapcontext(&scheduler_context, &(main_thread->context));
				}
				delete(temp);
				wait_thread = find_by_tid(current_thread->wait_tid);
				wait_thread->suspended_queue.push(activeID);
			}
			else {
				/*To do here*/
			}	
		}
	}
	else {
		if (schedule_policy == FIFO) {
			nextID = choose_next_thread_FIFO();
			if (nextID == NOT_FOUND) {
				makecontext(&scheduler_context, my_scheduler, 0);
				swapcontext(&scheduler_context, &(main_thread->context));
			}
		}
		else if (schedule_policy == SJF) {
			thread_PRI_SJF_FIFO* temp = NULL;
			temp = choose_next_thread_SJF();
			if (temp == NULL) {
				makecontext(&scheduler_context, my_scheduler, 0);
				swapcontext(&scheduler_context, &(main_thread->context));
			}
			nextID = temp->id;
			delete(temp);
		}
		else {
			nextID = choose_next_thread_PRI();
			if (nextID == NOT_FOUND) {
				makecontext(&scheduler_context, my_scheduler, 0);
				swapcontext(&scheduler_context, &(main_thread->context));
			}
		}
	}
	next_thread = find_by_tid(nextID);
	next_thread->active = TRUE;
	next_thread->status = SCHEDULED;
	current_active = next_thread;
	set_start_time(next_thread);
	makecontext(&scheduler_context, my_scheduler, 0);
	swapcontext(&scheduler_context, &next_thread->context);
}



































