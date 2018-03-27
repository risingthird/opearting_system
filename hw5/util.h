#ifndef UTIL_H
#define	UTIL_H

#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>


#define FREE 0
#define ALLOCATED 1
#define BLOCK_HEADER 29
#define GLOBAL_SIZE 40
#define BLOCK_SIZE 8
#define FALSE 0
#define TRUE 1
#define NO_SPACE            1
#define CORRUPT_FREESPACE   2
#define PADDING_OVERWRITTEN 3
#define BAD_ARGS            4
#define BAD_POINTER         5
#define RETURN_WITH_ERROR -1
#define RETURN_SUCCESS 0
#define STACK_CANARY 25

typedef struct A_Block {
    char canary;
    struct A_Block* next;
    struct A_Block* prev;       
    struct A_Block* next_free;
    int status;  
} Node;

typedef struct {
    Node* head;
    Node* head_free;
    long actual_size;
    long size_of_region;
    long allocated_size;
} Head;

Head* global_head;


int round_to(int val, int base);
int is_valid_addr(void*);
long get_block_size(void*);
Node* coalesce(Node*);
Node* get_header(void*);
char* perror(int error);














#endif