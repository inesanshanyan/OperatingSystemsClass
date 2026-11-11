#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

#define LINE_SIZE 4096

static int read_line(char *buf, int size, int *has_newline) {
    int i = 0;

    *has_newline = 0;

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
        if (n == 0) {
            break;
        }
        buf[i++] = c;
        if (c == '\n') {
            *has_newline = 1;
            break;
        }
    }

    buf[i] = '\0';
    return i;
}

int main(void) {
    char line[LINE_SIZE];
    char out[LINE_SIZE + 64];
    int has_newline = 0;
    int fd;
    int len;
    off_t offset;
    ssize_t written;

    printf("Enter a log line: ");
    fflush(stdout);
    if (read_line(line, LINE_SIZE, &has_newline) < 0) {
        return 1;
    }

    fd = open("log.txt", O_WRONLY | O_CREAT | O_APPEND, 0644);
    if (fd < 0) {
        perror("open");
        return 1;
    }

    if (has_newline) {
        len = snprintf(out, sizeof(out), "PID=%d: %s", (int)getpid(), line);
    } else {
        len = snprintf(out, sizeof(out), "PID=%d: %s\n", (int)getpid(), line);
    }
    if (len < 0 || (size_t)len >= sizeof(out)) {
        fprintf(stderr, "log line is too long\n");
        close(fd);
        return 1;
    }

    written = 0;
    while (written < len) {
        ssize_t n = write(fd, out + written, (size_t)(len - written));
        if (n < 0) {
            if (errno == EINTR) {
                continue;
            }
            perror("write");
            close(fd);
            return 1;
        }
        written += n;
    }

    /*
     * O_APPEND does not freeze the file offset. It only makes each write
     * start at the current end of the file. After the write finishes, the
     * kernel sets the offset to the byte just past what was written.
     * lseek(fd, 0, SEEK_CUR) reads that offset, so it grows on every append.
     */
    offset = lseek(fd, 0, SEEK_CUR);
    if (offset < 0) {
        perror("lseek");
        close(fd);
        return 1;
    }

    printf("File offset: %lld\n", (long long)offset);

    if (close(fd) < 0) {
        perror("close");
        return 1;
    }

    return 0;
}
