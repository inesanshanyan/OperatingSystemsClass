#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

#define REST_SIZE 1024

int main(void) {
    char rest[REST_SIZE];
    int fd;
    int i;
    int newlines;
    ssize_t n;
    ssize_t rest_len;
    ssize_t old_line_len;
    off_t start;
    off_t end;

    fd = open("numbers.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) {
        perror("open");
        return 1;
    }

    for (i = 1; i <= 10; i++) {
        char line[16];
        int len = snprintf(line, sizeof(line), "%d\n", i);

        if (len < 0 || write(fd, line, (size_t)len) != len) {
            perror("write");
            close(fd);
            return 1;
        }
    }

    if (close(fd) < 0) {
        perror("close");
        return 1;
    }

    fd = open("numbers.txt", O_RDWR);
    if (fd < 0) {
        perror("open");
        return 1;
    }

    newlines = 0;
    while ((n = read(fd, rest, 1)) == 1) {
        if (rest[0] == '\n') {
            newlines++;
            if (newlines == 3) {
                break;
            }
        }
    }
    if (n < 0) {
        perror("read");
        close(fd);
        return 1;
    }
    if (newlines < 3) {
        fprintf(stderr, "file has fewer than 4 lines\n");
        close(fd);
        return 1;
    }

    start = lseek(fd, 0, SEEK_CUR);
    if (start < 0) {
        perror("lseek");
        close(fd);
        return 1;
    }

    rest_len = read(fd, rest, sizeof(rest));
    if (rest_len < 0) {
        perror("read");
        close(fd);
        return 1;
    }
    if (rest_len == (ssize_t)sizeof(rest)) {
        fprintf(stderr, "remainder is too large\n");
        close(fd);
        return 1;
    }

    old_line_len = 0;
    while (old_line_len < rest_len && rest[old_line_len] != '\n') {
        old_line_len++;
    }
    if (old_line_len < rest_len) {
        old_line_len++;
    }

    if (lseek(fd, start, SEEK_SET) < 0) {
        perror("lseek");
        close(fd);
        return 1;
    }

    if (write(fd, "100\n", 4) != 4) {
        perror("write");
        close(fd);
        return 1;
    }
    if (write(fd, rest + old_line_len, (size_t)(rest_len - old_line_len)) != rest_len - old_line_len) {
        perror("write");
        close(fd);
        return 1;
    }

    end = lseek(fd, 0, SEEK_CUR);
    if (end < 0) {
        perror("lseek");
        close(fd);
        return 1;
    }
    if (ftruncate(fd, end) < 0) {
        perror("ftruncate");
        close(fd);
        return 1;
    }

    if (lseek(fd, 0, SEEK_SET) < 0) {
        perror("lseek");
        close(fd);
        return 1;
    }

    while ((n = read(fd, rest, sizeof(rest))) > 0) {
        if (write(1, rest, (size_t)n) != n) {
            perror("write");
            close(fd);
            return 1;
        }
    }
    if (n < 0) {
        perror("read");
        close(fd);
        return 1;
    }

    if (close(fd) < 0) {
        perror("close");
        return 1;
    }

    return 0;
}
