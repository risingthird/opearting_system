#include "util.h"


long round_to(int val, int base) {
	return ((val / base) * base + base);
}


// ask dianna about this
int is_valid_addr(void* pointer) {
	char* temp = (char*) pointer;
	if (temp > (char*) global_head->head && temp < (char*) global_head->head + global_head->actual_size) {
		return (pointer == (void*)(        (char*)((get_header(pointer))->status) + 7              ));
	}
	return FALSE;
}

long get_block_size(void* pointer) {
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

Node* get_header(void* pointer) {
	char* temp = (char*) pointer;
	temp -= BLOCK_HEADER;
	return pointer = temp;
}

Node* coalesce(Node* pointer) {
	// first coalease with next pointer if it exists and is free
	if (pointer->next != NULL && pointer->next->status == FREE) {
		pointer->next_free = pointer->next->next_free;
		pointer->next = pointer->next->next;
		if (pointer->next != NULL) {
			pointer->next->prev = pointer;
		}
	}

	// coalease with previous pointer if it exists and is free
	if (pointer->prev != NULL && pointer->prev->status == FREE) {
		pointer->prev->next_free = pointer->next_free;
		pointer->prev->next = pointer->next;
		if (pointer->next != NULL) {
			pointer->next->prev = pointer->prev;
		}
		return pointer->prev;
	}

	return pointer;
}

Node* coalesce_all(Node* pointer) {
	// first coalease with next pointer if it exists and is free
	if (pointer->next != NULL && pointer->next->status == FREE) {
		pointer->next_free = pointer->next->next_free;
		pointer->next = pointer->next->next;
		if (pointer->next != NULL) {
			pointer->next->prev = pointer;
		}
	}

	// coalease with previous pointer if it exists and is free
	if (pointer->prev != NULL && pointer->prev->status == FREE) {
		pointer->prev->next_free = pointer->next_free;
		pointer->prev->next = pointer->next;
		if (pointer->next != NULL) {
			pointer->next->prev = pointer->prev;
		}
		return pointer->prev;
	}

	return pointer;
}

// char* perror(int error) {
// 	if (error == NO_SPACE) {
// 		return "No space!\n";
// 	}
// 	else if (error == CORRUPT_FREESPACE) {
// 		return "Corrupt free space!\n";
// 	}
// 	else if (error == PADDING_OVERWRITTEN) {
// 		return "Padding overwritten!\n";
// 	}
// 	else if (error == BAD_ARGS) {
// 		return "Bad arguments!\n";
// 	}
// 	else if (error == BAD_POINTER) {
// 		return "Bad pointer!\n";
// 	}
// 	else {
// 		return "Undefined behavior!\n";
// 	}
// }
































