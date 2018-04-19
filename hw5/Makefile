SHELL:=/bin/bash -O extglob




mem.o : mem.c util.h
	gcc -Wall -pedantic -fpic -g -c -Daligned mem.c

libmem.so : mem.o
	gcc -o libmem.so mem.o -shared -g

simple_8_byte_allocation: simple_8_byte_allocation.c libmem.so
	gcc -o simple_8_byte_allocation simple_8_byte_allocation.c -L. -lusermem -g

check_for_8_byte_alignment: check_for_8_byte_alignment.c libmem.so
	gcc -o check_for_8_byte_alignment check_for_8_byte_alignment.c -L. -lusermem -g

coalesce_of_free_space: coalesce_of_free_space.c libmem.so
	gcc -o coalesce_of_free_space coalesce_of_free_space.c -L. -lusermem -g

two_mil_with_coalescing: two_mil_with_coalescing.c libmem.so
	gcc -o two_mil_with_coalescing two_mil_with_coalescing.c -L. -lusermem -g

worstfit_allocation: worstfit_allocation.c libmem.so
	gcc -o worstfit_allocation worstfit_allocation.c -L. -lusermem -g

global_coalesce: global_coalesce.c libmem.so
	gcc -o global_coalesce global_coalesce.c -L. -lusermem -g

bad_args_to_Mem_Init: bad_args_to_Mem_Init.c libmem.so
	gcc -o bad_args_to_Mem_Init bad_args_to_Mem_Init.c -L. -lusermem -g

several_odd_sized_allocations: several_odd_sized_allocations.c libmem.so
	gcc -o several_odd_sized_allocations several_odd_sized_allocations.c -L. -lusermem -g

a_few_aligned_allocations: a_few_aligned_allocations.c libmem.so
	gcc -o a_few_aligned_allocations a_few_aligned_allocations.c -L. -lusermem -g

simple_allocation_and_free: simple_allocation_and_free.c libmem.so
	gcc -o simple_allocation_and_free simple_allocation_and_free.c -L. -lusermem -g

aligned_allocation_and_free: aligned_allocation_and_free.c libmem.so
	gcc -o aligned_allocation_and_free aligned_allocation_and_free.c -L. -lusermem -g

odd_sized_allocation_and_free: odd_sized_allocation_and_free.c libmem.so
	gcc -o odd_sized_allocation_and_free odd_sized_allocation_and_free.c -L. -lusermem -g

initialize_with_size_of_1_page: initialize_with_size_of_1_page.c libmem.so
	gcc -o initialize_with_size_of_1_page initialize_with_size_of_1_page.c -L. -lusermem -g

initialize_and_round_up_to_1_page: initialize_and_round_up_to_1_page.c libmem.so
	gcc -o initialize_and_round_up_to_1_page initialize_and_round_up_to_1_page.c -L. -lusermem -g

no_space_left_to_allocate: no_space_left_to_allocate.c libmem.so
	gcc -o no_space_left_to_allocate no_space_left_to_allocate.c -L. -lusermem -g

try_to_free_a_NULL_pointer: try_to_free_a_NULL_pointer.c libmem.so
	gcc -o try_to_free_a_NULL_pointer try_to_free_a_NULL_pointer.c -L. -lusermem -g

check_memory_can_be_written_to: check_memory_can_be_written_to.c libmem.so
	gcc -o check_memory_can_be_written_to check_memory_can_be_written_to.c -L. -lusermem -g

two_mil_without_coalescing: two_mil_without_coalescing.c libmem.so
	gcc -o two_mil_without_coalescing two_mil_without_coalescing.c -L. -lusermem -g


clean: 
	@rm -f !(*.cpp|Makefile|README.md|*.h|BMC-CS355|*.c|*.txt)
















































