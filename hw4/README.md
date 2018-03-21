Name : Jiaping Wang

How to complie: use "make userthread.o" command to compile userthread.cpp, and "make libuserthread.so " command to compile the shared library.
				Note that not all tests are supported by makefile, grader might need to manually compile tests in order to run.
				Also, before linking test file to library, grader needs to run "export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:." command, otherwise tests cannot be run.
				Use make clean to clear up all generated files except from logger file.
				To examine logger file, graders might want to check log_file.txt.
				Caution: libraries are writtern in cpp, please use g++ to compile if you want to compile manually.


Extra credit:  In userthread_aging.cpp, aging shortest job first policy is implmented. Use "userthread_aging.o" command and "libuserthread_aging.so" command to compile library.


How to run: After linking testing file to user thread library, use ./xxxxx to run test file as usual. Remember to "export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:." before running test.

Limitation: No time for memcheck, let's hope there's no mem errors or mem leaks, which is 99.999999999999% impossible.
			Doesn't support multiple join (i.e a thread joining in more than one thread)
			Doesn't support circular join (i.e a->b->c->a)

Test files from Jiaping:
	wjp_priority_yield_same.c : 
		A simple test testing yield when the policy is priority, but every threads have the same priority, in other words, similar to fifo.

	wjp_sjf_yield.c:
		A simple test testing yield with sjf policy

	wjp_priority_yield.c:
		A simple test testing yield with priority policy

	wjp_test_fifo_basic.c:
		A simple test testing yield with fifo policy

	wjp_test_invalid_join.c:
		A simple test testing joining into a finished thread

	wjp_test_invalid_join2.c:
		A simple test testing joining into a non-existing thread

	wjp_test_invalid_policy.c:
		A simple test testing initializing library with an invalid policy

	wjp_test_invalid_priority.c:
		A simple test testing creating a thread with an invalid priority

	wjp_test_lib_init.c:
		A simple test testing libinit and libterminate

	wjp_test_misuse1.c:
		A simple test testing some misuses of library function such as terminate library before initializing










File directory: /Desktop/cs355/opearting_system/hw4