#include <stdio.h>
#include <stddef.h>

struct Normal {
    char a;
    int b;
    double c;
};

#pragma pack(1)
struct Packed1 {
    char a;
    int b;
    double c;
};
#pragma pack()

#pragma pack(2)
struct Packed2 {
    char a;
    int b;
    double c;
};
#pragma pack()

#pragma pack(4)
struct Packed4 {
    char a;
    int b;
    double c;
};
#pragma pack()

static void print_fields(const char *name, size_t size, size_t align,
                         const void *a, size_t off_a,
                         const void *b, size_t off_b,
                         const void *c, size_t off_c) {
    printf("%s\n", name);
    printf("sizeof = %zu  alignment = %zu\n", size, align);
    printf("a address = %p  offset = %zu\n", a, off_a);
    printf("b address = %p  offset = %zu\n", b, off_b);
    printf("c address = %p  offset = %zu\n", c, off_c);
    printf("padding before b = %zu, before c = %zu, after c = %zu\n\n",
           off_b - off_a - sizeof(char),
           off_c - off_b - sizeof(int),
           size - off_c - sizeof(double));
}

int main(void) {
    struct Normal normal;
    struct Packed1 packed1;
    struct Packed2 packed2;
    struct Packed4 packed4;

    printf("Fields: char a, int b, double c\n");
    printf("Useful data only: %zu bytes\n\n", sizeof(char) + sizeof(int) + sizeof(double));

    print_fields("Without #pragma pack",
                 sizeof(normal), _Alignof(struct Normal),
                 &normal.a, offsetof(struct Normal, a),
                 &normal.b, offsetof(struct Normal, b),
                 &normal.c, offsetof(struct Normal, c));

    print_fields("With #pragma pack(1)",
                 sizeof(packed1), _Alignof(struct Packed1),
                 &packed1.a, offsetof(struct Packed1, a),
                 &packed1.b, offsetof(struct Packed1, b),
                 &packed1.c, offsetof(struct Packed1, c));

    print_fields("With #pragma pack(2)",
                 sizeof(packed2), _Alignof(struct Packed2),
                 &packed2.a, offsetof(struct Packed2, a),
                 &packed2.b, offsetof(struct Packed2, b),
                 &packed2.c, offsetof(struct Packed2, c));

    print_fields("With #pragma pack(4)",
                 sizeof(packed4), _Alignof(struct Packed4),
                 &packed4.a, offsetof(struct Packed4, a),
                 &packed4.b, offsetof(struct Packed4, b),
                 &packed4.c, offsetof(struct Packed4, c));

    return 0;
}
