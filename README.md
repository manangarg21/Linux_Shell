# Description

## This is a simple representation of the bash shell in C . 
### To start the shell steps to follow:
1. Download the zip file and extract it.
2. On the terminal run make.
3. Do ./a.out in the terminal and you will launch the terminal.

### Now to implement the commands:
**activities**
1. Run activities on your terminal to check which processes are in the background along with their pid and which state they are in (Stopped/Running)

**bg**
1. Run bg <pid> of the background process which is stopped and it will start running it in the background again

**fg**
1. Run fg <pid> of the background process which is either stopped or running to make it run in the foreground

**iMan**
1. run iMan <command> to get name, synopsis and description of the command you wish to see (extracts data from man.he.net)

**neonate**
1. run neonate -n <time_val> to get the most recent pid's every time_val seconds until you press x to exit the raw mode.

**pastevents**
1. Creates a file history.txt in which the 15 most recent commands get stored
2. You can use **pastevents purge** to clear the history.txt and clear your pastevents
3. You can use **pastevents execute <num>** which will execute the <num>th most recent command

**peek**
1. peek command lists all the files and directories in the specified directories in lexicographic order.
2. run **peek <flags> <path>**
3. **flags**
-l : displays extra information (the total block size, the permissions of the file, file/directory name, owner name, file size, last edited)
-a : displays hidden files
-la , -al , -l -a , -a -l : displays extra information of all files(including hidden files)
no flags : display files (no hidden files)
4. **path**
can be both relative or absolute
5. Colours according to file type
green - executables
white - normal files
blue - directories

**ping**
1. run ping <pid> <signal_number>. Sends signal to the process with the given pid to change according to the signal number.
2. **Ctrl+C** - kills the current running foreground process
3. **Ctrl+Z** - puts the running foreground process into the background process into a stopped state.
4. **Ctrl+D** - exits the terminal

**pipe**
1. run <cmd 1>|<cmd 2>|... . It takes the output from command on left and passes it as standard input to the command on right.

**proc**
1. run proclore <pid> to get information about the process with the given pid (pid,process status,process group,virtual memory,executable path of the process)

**prompt**
1. returns the name of the directory you are currently in (absolute or relative)

**redirect**
1. handles the input-output redirection
2. **<** - Reads input from the filename following “<”.
3. **>** - Outputs to the filename following “>”.
4. **>>** - Similar to “>” but appends instead of overwriting if the file already exists.

**seek**
1. run seek <flags><search><target_directory> to search for the file/directory (depending on the flags) in the target directory.
2. **flags**
-d : Only look for directories (ignore files even if name matches)
-e : If only one file (and no directories) is found, then print it’s output. If only one directory (and no files) is found, then change current working directory to it.
-f : Only look for files (ignore directories even if name matches)
no flags : Gives all files and directories with the name in the target directory.
3. **path**
can be both relative or absolute

**warp**
1. run warp <directory> to go to the given directory. (can be absolute or relative).

**Some commands which are not executed through this are executed through execvp like echo, sleep, clear etc.**

# Assumptions

1. The max command size can be 4096 bytes.
2. All the C files are getting integrated in makefile and each .c file has its .h file included in headers.h
3. I have created a history file for pastevents which should not be touched once created.
4. Seek function is also showing hidden files.
5. To execute pastevents execute you need to first run pastevents.
6. If you run pastevents execute <value> ; <command> it executes it once but doesnt get stored in the history.txt 
7. iMan works if there is author, return value, options or attribute in it and works till 100000 characters.
