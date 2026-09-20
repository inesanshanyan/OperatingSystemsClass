#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    pid_t child1, child2;
    int status;

    child1 = fork();

    if (child1 < 0) {
        printf("Error: first fork() failed\n");
        return 1;
    }

    if (child1 == 0) {
        printf("Child 1: PID = %d\n", getpid());
        exit(10);
    }

    child2 = fork();

    if (child2 < 0) {
        printf("Error: second fork() failed\n");
        return 1;
    }

    if (child2 == 0) {
        printf("Child 2: PID = %d\n", getpid());
        exit(20);
    }

    waitpid(child1, &status, 0);

    if (WIFEXITED(status)) {
        printf("Child 1 exited normally with status: %d\n",
               WEXITSTATUS(status));
    } else {
        printf("Child 1 did not exit normally\n");
    }

    waitpid(child2, &status, 0);

    if (WIFEXITED(status)) {
        printf("Child 2 exited normally with status: %d\n",
               WEXITSTATUS(status));
    } else {
        printf("Child 2 did not exit normally\n");
    }

    printf("Parent process: PID = %d\n", getpid());

    return 0;
}