#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

int main() {
    printf("Initial process: PID = %d, PPID = %d\n",
           getpid(), getppid());

    for (int i = 0; i < 3; i++) {
        pid_t pid = fork();

        if (pid < 0) {
            printf("fork failed");
            exit(EXIT_FAILURE);
        }

        printf("Process: PID = %d, PPID = %d, fork return = %d\n",
               getpid(), getppid(), pid);
    }

    return 0;
}