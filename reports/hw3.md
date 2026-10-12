**Fork and Exec Assignments Report**

**Assignment 1**

The program calls fork() to create a child process. After fork(), both the parent and child continue executing the program, but fork() returns different values in each process. The child receives 0, while the parent receives the child's PID. The child prints its PID using getpid() and exits with exit(0). The parent prints its own PID without waiting for the child. This shows that fork() creates a separate process, and both processes can continue executing independently.

**Assignment 2**

The program creates two child processes using fork(). Child 1 sleeps for 2 seconds and exits with status 10, while Child 2 sleeps for 1 second and exits with status 20. The parent first calls waitpid() for Child 2, so it waits for that specific child to finish. After collecting its termination information, the parent calls wait() to wait for the remaining child.

The status variable stores encoded information about how a child terminated. WIFEXITED() checks whether the child exited normally, and WEXITSTATUS() extracts the exit code. This shows how wait() and waitpid() allow the parent to synchronize with its children and receive their termination status.

**Assignment 3**

The program registers two cleanup functions using atexit(). These functions are not executed immediately when they are registered. They are called automatically when the program terminates normally through exit(0). The functions execute in reverse order of registration, so cleanup2() runs before cleanup1().

This shows that atexit() allows a program to register functions that perform cleanup during normal termination. This can be useful for closing files, releasing resources, or saving information before the program finishes.

**Assignment 4**

The program creates two child processes using fork(). Each child exits with a different status code: Child 1 returns 10, and Child 2 returns 20. The parent uses waitpid() to wait for each specific child and stores the termination information in the status variable.

WIFEXITED() checks whether the child terminated normally, while WEXITSTATUS() extracts the exit code from the stored status information. The parent prints the exit status of each child after collecting it.

This shows how a parent process can track the termination of its children and check whether they exited normally or not.

## Assignment 5 

### 1. Creating a Zombie Process

The program creates a child process using fork(). The child prints its PID and exits with exit(0). The parent does not call wait(), so it continues running and sleeps for 20 seconds.

When the child terminates, the operating system keeps information about its exit status because the parent has not collected it yet. During this time, the child appears as a zombie process in the process table. The ps command can be used to observe its status as Z. This shows that a terminated process can remain in the process table until its parent collects its termination information.

### 2. Preventing a Zombie Process with wait()

The modified program uses wait(&status) in the parent process. After the child exits, the parent waits for it and collects its termination information. WIFEXITED() checks whether the child exited normally, and WEXITSTATUS() retrieves its exit code.

After wait() collects the child's status, the operating system can remove the child's zombie entry from the process table. This shows how wait() allows the parent to synchronize with the child and prevent a zombie process from remaining.

github link:  
<https://github.com/inesanshanyan/OperatingSystemsClass/tree/main/homeworks/hw3>
