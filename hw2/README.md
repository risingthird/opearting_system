Name: Jiaping Wang

How to complie: use 'make' command to compile three files at a time \
		If you want to compile a specific file, please use 'make matrix' \
		or 'make shell' or 'make shell_hist' command.

How to run: ./matrix will run matrix.c, no \
		arguments required \
	    For shell, there're two versions, ./shell can run basic version with no history implemented, ./shell_hist can run the version where history is implemented. However, for ./shell_hist, if user calls !! / !n / !-n, any further arguments will be ignored, in other words if the previous command is "cat file1", and user calls "!-1 file2", "cat file1 file2" won't be called as it will actually be in a real shell. In my shell, only "cat file1" will be called.

Limitation: As described above, any further arguments after "!!" / "!n" / "!-n" will be ignored. Also, commands like 
"cat file1> file2" or "cat file1 >file2" won't run as it will be in a real shell. In other words, commands are splited by tab or space, "file1>" will be considered as one argument rather than "file1" and ">".
"cd" command can be correctly stored in history, but cannot be executed.
Cannot handle ctrl-c, that is hitting on ctrl-c will terminate the shell rather than having a new prompt. 
Cannot handle '>', '>>', '&', command. It seems execvp cannot handle them properly.


File directory: /Desktop/cs355/opearting_system/hw2
