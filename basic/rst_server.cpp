#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>

int main() {
    int listen_fd, client_fd;
    int ret;
    struct sockaddr_in listen_addr, client_addr;
    socklen_t len;

    listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (listen_fd < 0) {
        perror("socket failed");
        return -1;
    }

    listen_addr.sin_family = AF_INET;
    listen_addr.sin_port = htons(8899);
    listen_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    ret = bind(listen_fd, (sockaddr *)&listen_addr, sizeof(listen_addr));
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
    client_fd = accept(listen_fd,(sockaddr *) &client_addr, &len);
    if (client_fd < 0) {
        perror("accept failed");
        return -1;
    }

    // 等待客户端发消息
    sleep(1);

    // 不读取消息，直接close, 观察tcpdump的结果
    close(client_fd);
}
