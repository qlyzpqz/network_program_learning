#include <unistd.h>
#include <stdio.h>
#include <netinet/in.h>
#include <errno.h>
#include <arpa/inet.h>
#include <string.h>
#include <sys/signal.h>

void pipe_handle(int signo) {
    printf("receive signal: %d\n", signo);
}

int main() {
    int fd;
    int ret;
    struct sockaddr_in addr;
    char *str = "hello world";
    char buf[10];

    signal(SIGPIPE, pipe_handle);

    fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0) {
        perror("create socket failed");
        return -1;
    }

    addr.sin_family = AF_INET;
    addr.sin_port = htons(8899);
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    ret = connect(fd, (sockaddr *)&addr, sizeof(addr));
    if (ret < 0) {
        perror("connect to server failed");
        return -1;
    }

    write(fd, str, strlen(str));
    // close(fd);

    /*
    while (true) {
        ret = read(fd, buf, sizeof(buf));
        if (ret == 0) {
            printf("read to end...");
        } else if (ret > 0) {
            continue;
        } else if (errno == EAGAIN) {
            continue;
        } else {
            perror("read failed");
            break;
        }
    }
    */

    sleep(2);

    ret = write(fd, buf, sizeof(buf));
    if (ret < 0) {
        perror("second read failed");
    }

    close(fd);
}
