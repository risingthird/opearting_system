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
			Doesn't support 