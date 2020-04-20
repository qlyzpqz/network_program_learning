#include <unistd.h>
#include <netinet/in.h>
#include <sys/epoll.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <fcntl.h>

#define MAX_POLLS 1000

void set_no_block(int fd) {
    int flags;
    flags = fcntl(fd, F_GETFL, 0);
    flags = flags | O_NONBLOCK;
    fcntl(fd, F_SETFL, flags);
}

int main() {
    int epoll_fd;
    int listen_fd, client_fd;
    int ret;
    struct sockaddr_in addr;
    struct sockaddr_in client_addr;
    struct epoll_event ev;
    struct epoll_event events[MAX_POLLS];
    int nfd;
    socklen_t len;

    epoll_fd = epoll_create(MAX_POLLS);
    if (epoll_fd < 0) {
        perror("epoll_create failed");
        return -1;
    }

    listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (listen_fd < 0) {
        perror("create socket failed");
        return -1;
    }

    addr.sin_family = AF_INET;
    addr.sin_port = htons(8899);
    inet_pton(AF_INET, "0.0.0.0", &addr.sin_addr);

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

    ev.events = EPOLLIN;
    ev.data.fd = listen_fd;
    epoll_ctl(epoll_fd, EPOLL_CTL_ADD, listen_fd, &ev);

    while (true) {
        nfd = epoll_wait(epoll_fd, events, MAX_POLLS, 0);
        for (int i = 0; i < nfd; i++) {
            if (events[i].data.fd == listen_fd) {
                client_fd = accept(listen_fd, (sockaddr *)&client_addr, &len);
                printf("accept new connection, client_fd=%d\n", client_fd);
            }
        }
    }
}
