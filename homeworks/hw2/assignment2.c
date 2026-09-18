#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    pid_t child1, child2;

    child1 = fork();

    if (child1 < 0) {
        printf("First fork failed");
        exit(EXIT_FAILURE);
    }

    if (child1 == 0) {
        execl("/bin/ls", "ls", (char *)NULL);

        printf("execl ls failed");
        exit(EXIT_FAILURE);
    }

    wait(NULL);

    child2 = fork();

    if (child2 < 0) {
        printf("Second fork failed");
        exit(EXIT_FAILURE);
    }

    if (child2 == 0) {
        execl("/bin/date", "date", (char *)NULL);
        printf("execl date failed");
        exit(EXIT_FAILURE);
    }


    wait(NULL);
    printf("Parent process done\n");

    return 0;
}