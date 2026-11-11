#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

#define PATH_SIZE 1024
#define BUF_SIZE 4096

static int read_line(char *buf, int size) {
    int i = 0;

    while (i < size - 1) {
        char c;
        ssize_t n = read(0, &c, 1);

        if (n < 0) {
            if (errno == EINTR) {
                continue;
            }
            perror("read");
            return -1;
        }
        if (n == 0 || c == '\n') {
            break;
        }
        buf[i++] = c;
    }

    buf[i] = '\0';
    return i;
}

static ssize_t read_chunk(int fd, unsigned char *buf, size_t count) {
    size_t got = 0;

    while (got < count) {
        ssize_t n = read(fd, buf + got, count - got);

        if (n < 0) {
            if (errno == EINTR) {
                continue;
            }
            return -1;
        }
        if (n == 0) {
            break;
        }
        got += (size_t)n;
    }

    return (ssize_t)got;
}

int main(void) {
    char path1[PATH_SIZE];
    char path2[PATH_SIZE];
    unsigned char buf1[BUF_SIZE];
    unsigned char buf2[BUF_SIZE];
    int fd1;
    int fd2;
    long long index = 0;

    printf("Enter first file path: ");
    fflush(stdout);
    if (read_line(path1, PATH_SIZE) < 0) {
        return 1;
    }

    printf("Enter second file path: ");
    fflush(stdout);
    if (read_line(path2, PATH_SIZE) < 0) {
        return 1;
    }

    fd1 = open(path1, O_RDONLY);
    if (fd1 < 0) {
        perror("open first file");
        return 1;
    }

    fd2 = open(path2, O_RDONLY);
    if (fd2 < 0) {
        perror("open second file");
        close(fd1);
        return 1;
    }

    for (;;) {
        ssize_t n1 = read_chunk(fd1, buf1, sizeof(buf1));
        ssize_t n2 = read_chunk(fd2, buf2, sizeof(buf2));
        ssize_t common;
        ssize_t i;

        if (n1 < 0 || n2 < 0) {
            perror("read");
            close(fd1);
            close(fd2);
            return 1;
        }

        common = n1 < n2 ? n1 : n2;
        for (i = 0; i < common; i++) {
            if (buf1[i] != buf2[i]) {
                printf("Files differ at byte %lld\n", index + (long long)i);
                close(fd1);
                close(fd2);
                return 1;
            }
        }

        if (n1 != n2) {
            printf("Files differ at byte %lld\n", index + (long long)common);
            close(fd1);
            close(fd2);
            return 1;
        }

        if (n1 == 0) {
            printf("Files are identical\n");
            break;
        }

        index += (long long)n1;
    }

    if (close(fd1) < 0) {
        perror("close");
        close(fd2);
        return 1;
    }
    if (close(fd2) < 0) {
        perror("close");
        return 1;
    }

    return 0;
}
