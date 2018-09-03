# OSAssignment_1

To create terminal based file explorer

Normal Mode

Funtionalities Implemented
1. Listing Directory
	1.1 Displays file type,permissions,owner,lastmodified,size,file name	
	1.2 Handling vertical overflow(Scrolling)
	1.3 Displays entries for '.' and '..' with appropriate functionality
	1.4 List navigation using up and down arrow key

2. Opening Files and Directories
	2.1 Opening directory and subsequently executing first functionality
	2.2 Opening file with default program using xdg-open 

3. Traversal 
	3.1 Goes back to previou directory upon pressing left arrow key
	3.2 Goes forward in directory structure upon pressing right arrow key	
	3.3 Goes one level up in directory structure upon pressing backspace key
	3.4 Goes to home directory upon pressing 'h' key

Command Mode

Funtionalities Implemented

1. Copy file : copy_file <file(s)> <destination directory>
	       Copy the files to the destination directory while maintaining the original copy

2. Move file : move_file <file(s)> <destination directory>
	       Move the files to the destination directory while deleting the original copy

3. Rename file : rename <source file> <new name>
		Renames the old file

4. Create File : create_file <file name> <destination directory>
		 Creates the file with the given filename in the destination directory			
	
5. Create Directory : create_dir <directory name> <destination directory>
		 Creates the new directory with the given directory name in the destination directory			

6. Delete File : delete_file <file name> <file path>
		 Deletes the particular file with the given file path

7. Delete Directory : delete_dir <directory> <directory path>
		      Deletes particular directory with the given path	

8. Goto Directory : goto <directory> 
                    Opens the directory and list all files an folders in that particular directory

7. Search : search <filename>
            Displays all the successful search matches on terminal in normal mode

8. Snapshot : snapshot <directory_name> <filename>
            It creates a dump file in with the name entered which contains entire directory structure from the given particular directory


Transition 

1. From Normal mode to Command mode : Enter ':' from console

2. From Command mode to Normal mode : Press ESC + ENTER


Note :

1. Error handling of opening a file whoes default program is unknown is not done

2. Copy directory and Move directory functionality not implemented 

3. Dynamic resizing of terminal not handled , user is expected to run the program in full screen mode

4. Snapshot is a seperate program which is executed by main application via fork and execl , user is expected to compile snapshot.cpp file with the following command
	
	g++ snapshot.cpp -o snapshot





























