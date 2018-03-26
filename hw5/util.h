#ifndef UTIL_H
#define	UTIL_H

#include "mem.h"

#define FREE 0
#define ALLOCATED 1
#define BLOCK_HEADER 28
#define GLOBAL 32
#define FALSE 0
#define TRUE 1

typedef struct A_Block {
    struct A_Block* next;
    struct A_Block* prev;       
    struct A_Block* next_free;
    int status;  
    char data[1];
} Node;

typedef struct {
    Node* head;
    Node* head_free;
    int actual_size;
    int size_of_region;
    int allocated_size;
} Head;

Head* global_head;


int round_to(int val, int base);
int is_valid_addr(void*);
Node* coalease(Node*);
Node* get_header(void*);
char* perror(int error);














#endif