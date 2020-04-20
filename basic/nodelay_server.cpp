#include <unistd.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <errno.h>

int main() {
    int listen_fd, client_fd;
    struct sockaddr_in addr, client_addr;
    socklen_t len;
    int ret;
    char buf[1024];

    listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (listen_fd < 0) {
        perror("create socket failed");
        return -1;
    }

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    addr.sin_port = htons(8899);

    ret = bind(listen_fd, (sockaddr *)&addr, sizeof(addr));
    if (ret < 0) {
        perror("bind failed");
        return -1;
    }

    ret = listen(listen_fd, 1024);
    if (ret < 0) {
        perror("listen failed");
        return -1;
    }

    len = sizeof(client_addr);
    client_fd = accept(listen_fd, (sockaddr *)&client_addr, &len);
    if (client_fd < 0) {
        perror("accept failed");
        return -1;
    }

    while (true) {
        ret = read(client_fd, buf, sizeof(buf));
        if (ret > 0) {
            printf("read from client, count=%d\n", ret);
        } else if (ret == 0) {
            break;
        } else if (errno == EAGAIN){
            continue;
        } else {
            perror("read failed");
            break;
        }
    }

    return 0;
}
