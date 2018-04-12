#ifndef DEFRAG_H
#define	DEFRAG_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>


#define N_DBLOCKS 10
#define N_IBLOCKS 4
#define INT_SIZE 4
#define EXIT_SUCCESS 0
#define EXIT_ERROR -1
#define BOOT_SIZE 512
#define SUPER_SIZE 512
#define TRUE 1
#define FALSE 0

typedef struct superblock {
	int size; /* size of blocks in bytes */
	int inode_offset; /* offset of inode region in blocks */
	int data_offset; /* data region offset in blocks */
	int swap_offset; /* swap region offset in blocks */
	int free_inode; /* head of free inode list, index */
	int free_block; /* head of free block list, index */
} SuperBlock;

int BLOCK_SIZE;
int INODE_SIZE;
int I_OFFSET;
int D_OFFSET;
int DATA_BEGIN;
int INODE_BEGIN;
int FILE_SIZE;
int block_index;
int num_inode;
int finished;
void* whole_buffer;
void* dest_buffer;
FILE* in_file;
FILE* out_file;
SuperBlock* sb;
void print_usage();




typedef struct inode {
	int next_inode; /* index of next free inode */
	int protect; /* protection field */
	int nlink; /* number of links to this file */
	int size; /* numer of bytes in file */
	int uid; /* owner’s user ID */
	int gid; /* owner’s group ID */
	int ctime; /* change time */
	int mtime; /* modification time */
	int atime; /* access time */
	int dblocks[N_DBLOCKS]; /* pointers to data blocks */
	int iblocks[N_IBLOCKS]; /* pointers to indirect blocks */
	int i2block; /* pointer to doubly indirect block */
	int i3block; /* pointer to triply indirect block */
} Inode;








#endif