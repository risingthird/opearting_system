#include "util.h"
#include "mem.h"

extern Head* global_head;
int m_error;
Node* largest;
Node* second_largest;
long last_block_size;


int Mem_Init(long sizeOfRegion) {
	if (global_head != NULL) {
		m_error = E_BAD_ARGS;
		return RETURN_WITH_ERROR;
	}

	if (sizeOfRegion <= 0 ) {
		m_error = E_BAD_ARGS;
		return RETURN_WITH_ERROR;
	}

	long to_alloc = round_to(sizeOfRegion, BLOCK_SIZE) / BLOCK_SIZE * BLOCK_HEADER + sizeOfRegion;
	long to_alloc_with_global = round_to(to_alloc + GLOBAL_SIZE, getpagesize());

	if ( (global_head = mmap(NULL, to_alloc_with_global, PROT_READ | PROT_WRITE,  MAP_PRIVATE | MAP_ANONYMOUS, -1, 0)) == MAP_FAILED) {
		m_error = E_NO_SPACE;
		return RETURN_WITH_ERROR;
	}
	global_head->actual_size = to_alloc_with_global;
	global_head->remaining_size = sizeOfRegion;
	global_head->head = (Node*) ((char*) global_head + GLOBAL_SIZE);
	global_head->head->next = NULL;
	global_head->head->prev = NULL;
	global_head->head->next_free = NULL;
	global_head->head->status = FREE;
	global_head->head->canary = STACK_CANARY;
	global_head->head_free = global_head->head;
	last_block_size = round_to(sizeOfRegion, BLOCK_SIZE);
	largest = NULL;
	second_largest = NULL;

	return RETURN_SUCCESS;

}

void *Mem_Alloc(long size) {
	if (size > global_head->remaining_size) {
		m_error = E_BAD_ARGS;
		return NULL;
	}

	size = round_to(size, BLOCK_SIZE);

	Node* curr = global_head->head_free;
	Node* prev_free = NULL;
	Node* temp1 = NULL;
	Node* temp2 = NULL;
	Node* next_to_allocate = NULL;
	int size_to_allocate = -1;
	int block_available = -1;
	if (largest != NULL && second_largest != NULL) {
		if (get_block_size(largest) >= get_block_size(second_largest)) {
			curr = NULL;
			next_to_allocate = largest;
			prev_free = largest->prev;
			size_to_allocate = get_block_size(largest);
			while (prev_free != NULL && prev_free->status != FREE) {
				prev_free = prev_free->prev;
				// if (prev_free->canary != STACK_CANARY) {
				// 	m_error = E_CORRUPT_FREESPACE;
				// 	return RETURN_WITH_ERROR;
				// }
			}
			
		}
	}
	else {
		largest = global_head->head_free;
		second_largest = global_head->head_free;
	}
	while(curr != NULL) {
		// if (curr->canary != STACK_CANARY) {
		// 	m_error = E_CORRUPT_FREESPACE;
		// 	return RETURN_WITH_ERROR;
		// }

		block_available = get_block_size(curr);

		if (block_available >= get_block_size(largest)) {
			second_largest = largest;
			largest = curr;
		}
		else if (block_available >= get_block_size(second_largest)) {
			second_largest = curr;
		}

		if (block_available > size && block_available > size_to_allocate) {
			size_to_allocate = block_available;
			prev_free = temp1;
			next_to_allocate = curr;
		}
		temp1 = curr; // got some problems here
		curr = curr->next_free;
	}

	if (size_to_allocate == -1) {
		m_error = E_NO_SPACE;
		return NULL;
	}

	long real_block_size = get_real_block_size(next_to_allocate);

	if (real_block_size >= BLOCK_HEADER + size + BLOCK_SIZE) {
		temp2 = next_to_allocate->next;
		Node* new_next = (char*) (next_to_allocate) + BLOCK_HEADER + size;
		new_next->next = temp2;
		//printf("NEW NEXT IS HERE %p\n\n",new_next);
		
		if (temp2 != NULL) {
			temp2->prev = new_next;
		}

		new_next->prev = next_to_allocate;
		new_next->status = FREE;
		new_next->next_free = next_to_allocate->next_free;
		new_next->canary = STACK_CANARY;

		if (prev_free != NULL) {
			prev_free->next_free = new_next;
		}
		else {
			global_head->head_free = new_next;
		}
		next_to_allocate->next = new_next;
		if (get_block_size(new_next) > get_block_size(second_largest)) {
			largest = new_next;
		}
		else {
			largest = second_largest;
			second_largest = NULL;
		}
		if (new_next->next == NULL) {
			last_block_size -= size;
		}
	}
	else {
		if (prev_free == NULL) {
			global_head->head_free = next_to_allocate->next_free;
		}
		if (next_to_allocate->next == NULL) {
			size_of_last_allocate = size;
		}
		largest = second_largest;
		second_largest = NULL;
	}

	next_to_allocate->next_free = NULL;
	next_to_allocate->status = ALLOCATED;
	global_head->remaining_size -= size;
	return (void*) ((char*)next_to_allocate + BLOCK_HEADER);

}

int Mem_Free(void *ptr, int coalesce) {
	if (ptr == NULL) {
		//printf("I died here\n");
		if (coalesce != FALSE) {
			coalesce_all();
			need_global_coalesce = FALSE;
		} 
		return RETURN_SUCCESS;
	}

	if (need_global_coalesce == TRUE && coalesce != 0) {
		coalesce_all();
		need_global_coalesce = FALSE;
	}

	if (coalesce == 0) {
		need_global_coalesce = TRUE;
	}

	// how to check whether a ptr is valid 
	if (is_valid_addr(ptr)) {
		//printf("I got here. Yay!\n");
		Node* curr = get_header(ptr);
		// if (curr->canary != STACK_CANARY) {
		// 	m_error = E_CORRUPT_FREESPACE;
		// 	return RETURN_WITH_ERROR;
		// }
		long to_free_size = -1;

		to_free_size = get_block_size(curr);
		
		if (to_free_size == RETURN_WITH_ERROR) {
			return 0;
		}

		if (curr->status == ALLOCATED) {
			if (curr->next ==  NULL) {
				global_head->remaining_size += size_of_last_allocate;
				size_of_last_allocate = -1;
			}
			else {
				global_head->remaining_size += to_free_size;
			}
		}
		else {
			RETURN_SUCCESS; // is already a free block, no need to free
		}

		long before = -1;
		long after = -1;
		int coalesce_flag = FALSE;
		if (coalesce) {			
			Node* temp = curr;
			before = get_block_size(temp);
			curr = my_coalesce(curr);
			after = get_block_size(curr);
			if (after - before == 0 || temp == curr) {
				coalesce_flag = TRUE;
			}

		}

		curr->status = FREE;
		curr->canary = STACK_CANARY;

		Node* prev_free = curr->prev;
		while (prev_free != NULL && prev_free->status != FREE) {
			prev_free = prev_free->prev;
			// if (prev_free->canary != STACK_CANARY) {
			// 	m_error = E_CORRUPT_FREESPACE;
			// 	return RETURN_WITH_ERROR;
			// }
		}

		if (prev_free != NULL) {
			if (!coalesce || coalesce_flag == TRUE) {
				curr->next_free = prev_free->next_free;
			}
			prev_free->next_free = curr;

		}
		else {
			if (!coalesce || coalesce_flag == TRUE) { 
				curr->next_free = global_head->head_free;
			}
			global_head->head_free = curr;
		}

		if (largest != NULL && second_largest != NULL) {
			if (get_block_size(curr) > get_block_size(largest)) {
				second_largest = largest;
				largest = curr;
			}
			else if (get_block_size(curr) > get_block_size(second_largest)) {
				second_largest = curr;
			}
		}
		return RETURN_SUCCESS;

	}
	else {
		return RETURN_WITH_ERROR;
	}
}

void Mem_Dump() {
	Node* temp = global_head->head;
	while (temp != NULL) {
		printf("Block is %s, and has %ld bytes memories\n", temp->status ? "allocated" : "free", get_block_size(temp));
		temp = temp->next;
	}
	return;
}

int Mem_delete_all() {
	long to_delete = global_head->actual_size;
	int result = munmap(global_head, to_delete);
	global_head = NULL;
	return result;
}


long round_to(int val, int base) {
	return (((val - 1) / base) * base + base);
}


// ask dianna about this
int is_valid_addr(void* pointer) {
	char* temp = (char*) pointer;
	if (temp > (char*) global_head->head && temp < (char*) global_head->head + global_head->actual_size) {
		//printf("I got here. Yay!\n");
		if (get_header(pointer)->status == ALLOCATED || get_header(pointer)->status == FREE) {
			//printf("I got here!\n");
			return TRUE;
		}
		else {
			return FALSE;
		}
		
	}
	return FALSE;
}

long get_block_size(Node* pointer) {
	long result = -1;
	
	if (pointer ==  NULL) {
		return RETURN_WITH_ERROR;
	}

	if (pointer->next != NULL) {
		result = (char*)(pointer->next) - ((char*)pointer + BLOCK_HEADER);
	}
	else {
		result = last_block_size;
	}

	return result;
}

Node* get_header(void* pointer) {
	char* temp = (char*) pointer;
	temp -= BLOCK_HEADER;
	return pointer = temp;
}

Node* my_coalesce(Node* pointer) {
	// first coalease with next pointer if it exists and is free
	if (pointer == NULL) {
		return NULL;
	}
	Node* curr = pointer;
	Node* curr2 = pointer;
	Node* next_free_block = NULL;
	long coalesced_size = 0;
	long cur_size = get_block_size(pointer);
	
	if (pointer->next != NULL && pointer->next->status == FREE) {
		pointer->next_free = pointer->next->next_free;
		pointer->next = pointer->next->next;
		if (pointer->next != NULL) {
			pointer->next->prev = pointer;
		}
		if (largest != NULL && pointer->next == largest) {
			largest = NULL;
		}
		else if (second_largest != NULL && pointer->next == second_largest) {
			second_largest = NULL;
		}
	}
	if (pointer->next == NULL) {
		last_block_size += cur_size; 
	}
	long cur_size_prev = get_block_size(pointer->prev);

	// coalease with previous pointer if it exists and is free
	if (pointer->prev != NULL && pointer->prev->status == FREE) {
		pointer->prev->next_free = pointer->next_free;
		pointer->prev->next = pointer->next;
		if (pointer->next != NULL) {
			pointer->next->prev = pointer->prev;
		}
		if (largest != NULL && pointer->prev == largest) {
			largest = NULL;
		}
		else if (second_largest != NULL && pointer->prev == second_largest) {
			second_largest = NULL;
		}
		if(pointer->prev->next == NULL) {
			last_block_size += cur_size_prev;
		}
		return pointer->prev;
	}

	return pointer;
}

Node* coalesce_all() {
	// first coalease with next pointer if it exists and is free
	Node* temp = global_head->head;
	while(temp != NULL) {
		if (temp->status == FREE) {
			temp = my_coalesce(temp);
		}
		temp = temp->next_free;
	}
}

long get_real_block_size(Node* pointer) {
	long result = -1;
	
	if (pointer ==  NULL) {
		return RETURN_WITH_ERROR;
	}

	if (pointer->next != NULL) {
		result = (char*)(pointer->next) - ((char*)pointer + BLOCK_HEADER);
	}
	else {
		result = (char*)(global_head) + global_head->actual_size - (char*) pointer - BLOCK_HEADER;
	}

	return result;
}






