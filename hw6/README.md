Name: Jiaping Wang


How to complie: 
	Use "make" command to compile, and use "./defrag <file_name>" to run the executable file.


How the program works:
	The program first copies the file into a buffer, and then iterate through the inode region. During this process, the program will copy inode into a temp buffer
	and redirect to the data block pointed by the inode. After copying data block into the temp buffer, the program will then update the pointer of inode to current 
	data block index. After traversing all inodes, the program will mark the next_index field in each free block to its correct number. At last, we write the buffer 
	into an output file.

Limitations:
	No known bugs.




