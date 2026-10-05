#include <stdio.h>
#include <stdlib.h>

int main() {
    int total = 3;
    char **strings = malloc(total * sizeof(char *));

    if (strings == NULL) {
        printf("Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < total; i++) {
        strings[i] = malloc(50 * sizeof(char));

        if (strings[i] == NULL) {
            printf("Memory allocation failed\n");
            exit(EXIT_FAILURE);
        }
    }

    printf("Enter 3 strings: ");
    for (int i = 0; i < total; i++) {
        scanf("%s", strings[i]);
    }

    printf("Strings: ");
    for (int i = 0; i < total; i++) {
        printf("%s ", strings[i]);
    }
    printf("\n");

    int old_total = total;
    total += 2;

    char **resized = realloc(strings, total * sizeof(char *));

    if (resized == NULL) {
        printf("Memory reallocation failed\n");
        exit(EXIT_FAILURE);
    }

    strings = resized;

    for (int i = old_total; i < total; i++) {
        strings[i] = malloc(50 * sizeof(char));

        if (strings[i] == NULL) {
            printf("Memory allocation failed\n");
            exit(EXIT_FAILURE);
        }
    }

    printf("Enter 2 more strings: ");
    for (int i = old_total; i < total; i++) {
        scanf("%s", strings[i]);
    }

    printf("All strings: ");
    for (int i = 0; i < total; i++) {
        printf("%s ", strings[i]);
    }
    printf("\n");

    for (int i = 0; i < total; i++) {
        free(strings[i]);
    }
    free(strings);

    return 0;
}
