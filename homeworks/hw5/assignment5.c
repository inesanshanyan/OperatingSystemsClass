#include <stdio.h>
#include <stdlib.h>

int main() {
    int n;

    printf("Enter the number of students: ");
    scanf("%d", &n);

    int *grades = malloc(n * sizeof(int));

    if (grades == NULL) {
        printf("Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    printf("Enter the grades: ");
    for (int i = 0; i < n; i++) {
        scanf("%d", &grades[i]);
    }

    int highest = grades[0];
    int lowest = grades[0];

    for (int i = 1; i < n; i++) {
        if (grades[i] > highest) {
            highest = grades[i];
        }
        if (grades[i] < lowest) {
            lowest = grades[i];
        }
    }

    printf("Highest grade: %d\n", highest);
    printf("Lowest grade: %d\n", lowest);

    free(grades);

    return 0;
}
