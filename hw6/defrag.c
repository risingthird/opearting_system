#include "defrag.h"





int main(int argc, char** argv) {
	char file_name[100];
	bzero(file_name, 100);
	
	if (argc != 2) {
		exit(EXIT_ERROR);
	}

	if (strlen(argv[1]) == 2) {
		if (argv[1][0] == '-') {
			if (argv[1][1] == 'h') {
				print_usage();
				exit(EXIT_SUCCESS);
			}
			else {
				exit(EXIT_ERROR);
			}
		}
	}

	if ((in_file = fopen(argv[1], "r")) == NULL) {
		exit(EXIT_ERROR);
	}

	strcpy(file_name, argv[1]);
	strcat(file_name, "-defrag");

	if ((out_file = fopen(file_name, "w")) == NULL) {
		exit(EXIT_ERROR);
	}

	//FILE* data_file = fopen("data1", "w");


	fseek(in_file, 0, SEEK_END);
	FILE_SIZE = ftell(in_file);
	fseek(in_file, 0, SEEK_SET);
	whole_buffer = malloc(FILE_SIZE);
	dest_buffer = malloc(FILE_SIZE);
	bzero(dest_buffer, FILE_SIZE);
	bzero(whole_buffer, FILE_SIZE);
	fread(whole_buffer, 1, FILE_SIZE, in_file);

	sb = (SuperBlock*) (whole_buffer + BOOT_SIZE);
	block_index = 0;
	BLOCK_SIZE = sb->size;
	INODE_BEGIN = BOOT_SIZE + SUPER_SIZE + sb->inode_offset * BLOCK_SIZE;
	DATA_BEGIN = BOOT_SIZE + SUPER_SIZE + sb->data_offset * BLOCK_SIZE;
	INODE_SIZE = sizeof(Inode);
	num_inode = (DATA_BEGIN - INODE_BEGIN) / INODE_SIZE;

	//fwrite(whole_buffer, 1, INODE_BEGIN, out_file);  // write everything before inode region into the output file
	memcpy(dest_buffer, whole_buffer, INODE_BEGIN);
	sb = (SuperBlock*) (dest_buffer + BOOT_SIZE);

	int this_file_size = 0;
	int num_int_in_block = BLOCK_SIZE / INT_SIZE;
	Inode* curr = NULL;
	Inode* ori_curr = NULL;
	int* curr_block_lv1 = NULL;
	int* curr_block_lv2 = NULL;
	int* curr_block_lv3 = NULL;
	// The outmost loop will iterate through inode region and check each Inode
	for (int i = 0; i < num_inode; i++) {
		//fwrite(whole_buffer + INODE_BEGIN + INODE_SIZE * i, 1, INODE_SIZE, out_file);
		this_file_size = 0;
		memcpy(dest_buffer + INODE_BEGIN + INODE_SIZE * i, whole_buffer + INODE_BEGIN + INODE_SIZE * i, INODE_SIZE); // copy inode into file
		curr = (Inode*) (dest_buffer + INODE_BEGIN + INODE_SIZE * i);
		ori_curr = (Inode*) (whole_buffer + INODE_BEGIN + INODE_SIZE * i);
		if (curr->nlink == 0) {
			continue;
		}
		else {
			finished = FALSE;
			this_file_size = curr->size;
			//First check direct block
			for (int j = 0; j < N_DBLOCKS; j++) {
				if (this_file_size <= 0) {
					finished = TRUE;
					break;
				}
				this_file_size -= BLOCK_SIZE;
				// copy data in the direct block into the to_write buffer
				memcpy(dest_buffer + DATA_BEGIN + block_index * BLOCK_SIZE, whole_buffer + DATA_BEGIN + (curr->dblocks)[j] * BLOCK_SIZE, BLOCK_SIZE);
				//fwrite(dest_buffer + DATA_BEGIN + block_index * BLOCK_SIZE, 1, BLOCK_SIZE, data_file);
				(curr->dblocks)[j] = block_index; // update index
				block_index++;
			}

			// Check lv1 indirect block
			if (finished == FALSE) {
				for (int j = 0; j < N_IBLOCKS; j++) {
					if (this_file_size <= 0 || finished == TRUE) {
						finished = TRUE;
						break;
					}
					memcpy(dest_buffer + DATA_BEGIN + block_index * BLOCK_SIZE, whole_buffer + DATA_BEGIN + (ori_curr->iblocks)[j] * BLOCK_SIZE, BLOCK_SIZE);
					curr_block_lv1 = (int*) (dest_buffer + DATA_BEGIN + block_index * BLOCK_SIZE);
					curr->iblocks[j] = block_index;
					block_index++;
					for (int k = 0; k < num_int_in_block; k++) {
						if (this_file_size <= 0) {
							finished = TRUE;
							break;
						}
						this_file_size -= BLOCK_SIZE;
						// copy data in the direct block into the to_write buffer
						memcpy(dest_buffer + DATA_BEGIN + block_index * BLOCK_SIZE, whole_buffer + DATA_BEGIN + curr_block_lv1[k] * BLOCK_SIZE, BLOCK_SIZE);
						//fwrite(dest_buffer + DATA_BEGIN + block_index * BLOCK_SIZE, 1, BLOCK_SIZE, data_file);
						curr_block_lv1[k] = block_index;
						block_index++;
					}
				}
			}

			// Check lv2 indirect block
			if (finished == FALSE) {
				memcpy(dest_buffer + DATA_BEGIN + block_index * BLOCK_SIZE, whole_buffer + DATA_BEGIN + ori_curr->i2block * BLOCK_SIZE, BLOCK_SIZE);
				curr_block_lv2 = (int*) (dest_buffer + DATA_BEGIN + block_index * BLOCK_SIZE);
				curr->i2block = block_index;
				block_index++;
				for (int j = 0; j < num_int_in_block; j++) {
					if (this_file_size <= 0 || finished == TRUE) {
						finished = TRUE;
						break;
					}
					memcpy(dest_buffer + DATA_BEGIN + block_index * BLOCK_SIZE, whole_buffer + DATA_BEGIN + curr_block_lv2[j] * BLOCK_SIZE, BLOCK_SIZE);
					curr_block_lv1 = (int*) (dest_buffer + DATA_BEGIN + block_index * BLOCK_SIZE);
					curr_block_lv2[j] = block_index;
					block_index++;
					for (int k = 0; k < num_int_in_block; k++) {
						if (this_file_size <= 0) {
							finished = TRUE;
							break;
						}
						this_file_size -= BLOCK_SIZE;
						// copy data in the direct block into the to_write buffer
						memcpy(dest_buffer + DATA_BEGIN + block_index * BLOCK_SIZE, whole_buffer + DATA_BEGIN + curr_block_lv1[k] * BLOCK_SIZE, BLOCK_SIZE);
						//fwrite(dest_buffer + DATA_BEGIN + block_index * BLOCK_SIZE, 1, BLOCK_SIZE, data_file);
						curr_block_lv1[k] = block_index;
						block_index++;
					}										
				}
			}

			// Check lv3 indirect block
			if (finished == FALSE) {
				memcpy(dest_buffer + DATA_BEGIN + block_index * BLOCK_SIZE, whole_buffer + DATA_BEGIN + ori_curr->i3block * BLOCK_SIZE, BLOCK_SIZE);
				curr_block_lv3 = (int*) (dest_buffer + DATA_BEGIN + block_index * BLOCK_SIZE);
				curr->i3block = block_index;
				block_index++;
				for (int j = 0; j < num_int_in_block; j++) {
					if (this_file_size <= 0 || finished == TRUE) {
						finished = TRUE;
						break;
					}
					memcpy(dest_buffer + DATA_BEGIN + block_index * BLOCK_SIZE, whole_buffer + DATA_BEGIN + curr_block_lv3[j] * BLOCK_SIZE, BLOCK_SIZE);
					curr_block_lv2 = (int*) (dest_buffer + DATA_BEGIN + block_index * BLOCK_SIZE);
					curr_block_lv3[j] = block_index;
					block_index++;
					for (int k = 0; k < num_int_in_block; k++) {
						if (this_file_size <= 0 || finished == TRUE) {
							finished = TRUE;
							break;
						}
						memcpy(dest_buffer + DATA_BEGIN + block_index * BLOCK_SIZE, whole_buffer + DATA_BEGIN + curr_block_lv2[k] * BLOCK_SIZE, BLOCK_SIZE);
						curr_block_lv1 = (int*) (dest_buffer + DATA_BEGIN + block_index * BLOCK_SIZE);
						curr_block_lv2[k] = block_index; 
						block_index++;
						for (int l = 0; l < num_int_in_block; l++) {
							if (this_file_size <= 0) {
								finished = TRUE;
								break;
							}
							this_file_size -= BLOCK_SIZE;
							// copy data in the direct block into the to_write buffer
							memcpy(dest_buffer + DATA_BEGIN + block_index * BLOCK_SIZE, whole_buffer + DATA_BEGIN + curr_block_lv1[l] * BLOCK_SIZE, BLOCK_SIZE);
							//fwrite(dest_buffer + DATA_BEGIN + block_index * BLOCK_SIZE, 1, BLOCK_SIZE, data_file);
							curr_block_lv1[l] = block_index;
							block_index++;
						}	
					}
				}				
			}
		}
	}

	// By now we finished copying block with data in it
	// And we should deal with free block
	sb->free_block = block_index;
	//void* temp_block = NULL;
	int* a_free_block = NULL;
	for (int i = block_index; i < sb->swap_offset - sb->data_offset; i++) {
		//temp_block = dest_buffer + DATA_BEGIN + i * BLOCK_SIZE;
		a_free_block = (int*) (dest_buffer + DATA_BEGIN + i * BLOCK_SIZE);
		if (i == sb->swap_offset - 1) {
			*a_free_block = -1;
		}
		else {
			a_free_block[0] = i + 1;
		}
	}

	memcpy(dest_buffer + sb->swap_offset * BLOCK_SIZE, whole_buffer + sb->swap_offset * BLOCK_SIZE, FILE_SIZE - sb->swap_offset * BLOCK_SIZE);
	fwrite(dest_buffer, 1, FILE_SIZE, out_file);

	free(whole_buffer);
	free(dest_buffer);

	fclose(in_file);
	fclose(out_file);


}


void print_usage() {}

















































































