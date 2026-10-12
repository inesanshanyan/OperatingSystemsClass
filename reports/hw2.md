# **Fork and Exec Assignments Report**

**Assignment 0 – Multiple Fork Calls**

The program calls fork() three times in sequence. Each fork() copies the current process, so after the first call there are 2 processes, after the second there are 4, and after the third there are 8.

The important part is that every process created by a previous fork() continues executing the next fork(). This is why the number of processes doubles instead of creating only three children.

fork() returns 0 in the child and the child's PID in the parent. If it fails, it returns -1. The parent-child relationships therefore form a process tree.

**Assignment 1 – Fork and Exec**

First, fork() creates a child process. From this point, the parent and child continue from the same place in the program, but they are separate processes.

The child receives 0 from fork() and calls execl(). execl() does not create another process. Instead, it replaces the child's current program with the ls program. The child therefore keeps its process ID, but its program code and memory are replaced by ls.

The parent receives the child's PID from fork() and calls wait(NULL). This makes the parent wait until the child finishes. After ls exits, the parent continues and prints Parent process done.

**Assignment 2 – Multiple Forks and Execs**

The parent first calls fork() to create the first child. The first child calls execl() and is replaced by the ls program.

The parent then uses wait(NULL), so it waits for the first child to finish before creating the second child. After that, the parent calls fork() again. The second child is replaced by the date program using execl().

Because wait(NULL) is used after each child is created, the output appears in the order: ls, then date, then the parent's message.

The two children are separate processes and both have the same original parent process.

**Assignment 3 – Fork and Exec with Arguments**

fork() first creates a child process. The child then calls execl() with /bin/echo and passes the text as an argument.

The arguments after the program name are passed to the new program as its command-line arguments. echo receives the string "Hello from the child process" and prints it.

As in Assignment 1, execl() replaces the child program instead of creating another process. The parent uses wait(NULL) so that it prints its message only after the child has finished.

**Assignment 4 – Fork and Exec with Command-Line Arguments**

The program first creates a child using fork(). The child calls execl() to replace itself with the grep program.

The arguments given to execl() are passed to grep: "main" is the search word and "test.txt" is the file to search. grep reads the file and prints only the lines containing main.

The parent waits using wait(NULL). This is why Parent process completed is printed after the matching lines from grep.

Repository link:

[*https://github.com/inesanshanyan/OperatingSystemsClass/tree/main/homeworks/hw2*](https://github.com/inesanshanyan/OperatingSystemsClass/tree/main/homeworks/hw2)
