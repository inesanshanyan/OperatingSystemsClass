#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    pid_t pid = fork();

    if (pid < 0) {
        printf("fork failed");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) {
        execl("/bin/ls", "ls", (char *)NULL);

        printf("execl failed");
        exit(EXIT_FAILURE);
    } else {
        wait(NULL);
        printf("Parent process done\n");
    }

    return 0;
}