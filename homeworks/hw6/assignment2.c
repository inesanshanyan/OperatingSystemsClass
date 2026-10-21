#include <stdio.h>
#include <stddef.h>

struct Order1 {
    char a;
    int b;
    double c;
};

struct Order2 {
    double c;
    int b;
    char a;
};

struct Order3 {
    char a;
    double c;
    int b;
};

int main(void) {
    struct Order1 s1;
    struct Order2 s2;
    struct Order3 s3;

    printf("Order 1: char a, int b, double c\n");
    printf("sizeof = %zu\n", sizeof(s1));
    printf("a address = %p  offset = %zu\n", (void *)&s1.a, offsetof(struct Order1, a));
    printf("b address = %p  offset = %zu\n", (void *)&s1.b, offsetof(struct Order1, b));
    printf("c address = %p  offset = %zu\n", (void *)&s1.c, offsetof(struct Order1, c));

    printf("\nOrder 2: double c, int b, char a\n");
    printf("sizeof = %zu\n", sizeof(s2));
    printf("c address = %p  offset = %zu\n", (void *)&s2.c, offsetof(struct Order2, c));
    printf("b address = %p  offset = %zu\n", (void *)&s2.b, offsetof(struct Order2, b));
    printf("a address = %p  offset = %zu\n", (void *)&s2.a, offsetof(struct Order2, a));

    printf("\nOrder 3: char a, double c, int b\n");
    printf("sizeof = %zu\n", sizeof(s3));
    printf("a address = %p  offset = %zu\n", (void *)&s3.a, offsetof(struct Order3, a));
    printf("c address = %p  offset = %zu\n", (void *)&s3.c, offsetof(struct Order3, c));
    printf("b address = %p  offset = %zu\n", (void *)&s3.b, offsetof(struct Order3, b));

    printf("\nField sizes alone: char %zu + int %zu + double %zu = %zu\n",
           sizeof(char), sizeof(int), sizeof(double),
           sizeof(char) + sizeof(int) + sizeof(double));

    return 0;
}
