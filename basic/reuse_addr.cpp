#include <unistd.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main() {
    int fd, client_fd;
    int ret;
    struct sockaddr_in addr;
    struct sockaddr_in client_addr;
    socklen_t len;
    int yes;

    fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0) {
        perror("create socket failed");
        return -1;
    }

    yes = 1;
    setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes));

    addr.sin_family = AF_INET;
    addr.sin_port = htons(8899);
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    ret = bind(fd, (sockaddr *)&addr, sizeof(addr));
    if (ret < 0) {
        perror("bind failed");
        return -1;
    }

    ret = listen(fd, 1024);
    if (ret < 0) {
        perror("listen failed");
        return -1;
    }

    client_fd = accept(fd, (sockaddr*)&client_addr, &len);
    if (client_fd < 0) {
        perror("accept failed");
        return -1;
    }
    close(client_fd);

    getchar();

    return 0;
}
