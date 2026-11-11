#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

#define PATH_SIZE 1024

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

int main(void) {
    char path[PATH_SIZE];
    int fd;
    off_t size;
    off_t pos;

    printf("Enter file path: ");
    fflush(stdout);
    if (read_line(path, PATH_SIZE) < 0) {
        return 1;
    }

    fd = open(path, O_RDONLY);
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

    for (pos = size - 1; pos >= 0; pos--) {
        char c;
        ssize_t n;

        if (lseek(fd, pos, SEEK_SET) < 0) {
            perror("lseek");
            close(fd);
            return 1;
        }

        n = read(fd, &c, 1);
        if (n < 0) {
            perror("read");
            close(fd);
            return 1;
        }
        if (n == 0) {
            break;
        }
        if (write(1, &c, 1) != 1) {
            perror("write");
            close(fd);
            return 1;
        }
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
