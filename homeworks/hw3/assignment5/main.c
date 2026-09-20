#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
    pid_t pid;

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
        printf("Parent is sleeping for 20 seconds \n");

        sleep(20);

        printf("Parent process finished\n");
    }

    return 0;
}