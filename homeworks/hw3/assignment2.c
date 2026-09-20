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
        sleep(2);
        exit(10);
    }

    child2 = fork();

    if (child2 < 0) {
        printf("Error: second fork() failed\n");
        return 1;
    }

    if (child2 == 0) {
        printf("Child 2: PID = %d\n", getpid());
        sleep(1);
        exit(20);
    }

    waitpid(child2, &status, 0);

    if (WIFEXITED(status)) {
        printf("Child 2 exited with status: %d\n",
               WEXITSTATUS(status));
    }

    wait(&status);

    if (WIFEXITED(status)) {
        printf("Child 1 exited with status: %d\n",
               WEXITSTATUS(status));
    }

    printf("Parent process: PID = %d\n", getpid());

    return 0;
}