#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int main(void) {
    const char *text = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    char buf[32];
    int fd;
    off_t size;
    ssize_t n;

    fd = open("data.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) {
        perror("open");
        return 1;
    }

    if (write(fd, text, 26) != 26) {
        perror("write");
        close(fd);
        return 1;
    }
    if (close(fd) < 0) {
        perror("close");
        return 1;
    }

    fd = open("data.txt", O_RDWR);
    if (fd < 0) {
        perror("open");
        return 1;
    }

    size = lseek(fd, 0, SEEK_END);
    if (size < 0) {
        perror("lseek");
        close(fd);
        return 1;
    }
    printf("Current size: %lld\n", (long long)size);

    if (ftruncate(fd, 10) < 0) {
        perror("ftruncate");
        close(fd);
        return 1;
    }

    size = lseek(fd, 0, SEEK_END);
    if (size < 0) {
        perror("lseek");
        close(fd);
        return 1;
    }
    printf("New size: %lld\n", (long long)size);

    if (lseek(fd, 0, SEEK_SET) < 0) {
        perror("lseek");
        close(fd);
        return 1;
    }

    fflush(stdout);

    n = read(fd, buf, sizeof(buf));
    if (n < 0) {
        perror("read");
        close(fd);
        return 1;
    }
    if (write(1, buf, (size_t)n) != n) {
        perror("write");
        close(fd);
        return 1;
    }
    if (write(1, "\n", 1) != 1) {
        perror("write");
        close(fd);
        return 1;
    }

    if (close(fd) < 0) {
        perror("close");
        return 1;
    }

    return 0;
}
