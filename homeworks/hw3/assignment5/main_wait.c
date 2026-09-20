#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    pid_t pid;
    int status;

    pid = fork();

    if (pid < 0) {
        printf("Error: fork() failed\n");
        return 1;
    }

    if (pid == 0) {
        printf("Child process: PID = %d\n", getpid());
        exit(0);
    } else {
        printf("Parent process: PID = %d\n", getpid());

        printf("Parent is waiting for the child...\n");

        wait(&status);

        if (WIFEXITED(status)) {
            printf("Child exited normally with status: %d\n",
                   WEXITSTATUS(status));
        }

        printf("Parent process finished\n");
    }

    return 0;
}