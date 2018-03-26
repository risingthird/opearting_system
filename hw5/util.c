#include "util.h"


int round_to(int val, int base) {
	return ((val / base) * base + base);
}

int is_valid_addr(void* pointer) {
	char* temp = (char*) pointer;
	if (temp > global_head->head && temp < global_head->head + global_head->actual_size) {
		return (pointer == (void*)(get_block(pointer))->data);
	}
	return FALSE;
}

Node* get_header(void* pointer) {
	char* temp = (char*) pointer;
	temp -= BLOCK_HEADER;
	return pointer = temp;
}

Node* coalease(Node* pointer) {
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

char* perror(int error) {
	if (error == E_NO_SPACE) {
		return "No space!\n";
	}
	else if (error == E_CORRUPT_FREESPACE) {
		return "Corrupt free space!\n";
	}
	else if (error == E_PADDING_OVERWRITTEN) {
		return "Padding overwritten!\n";
	}
	else if (error == E_BAD_ARGS) {
		return "Bad arguments!\n";
	}
	else if (error == E_BAD_POINTER) {
		return "Bad pointer!\n";
	}
	else {
		return "Undefined behavior!\n";
	}
}
































