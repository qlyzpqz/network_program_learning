#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <netinet/tcp.h>
#include <sys/select.h>

int sleep_ms(int ms) {
    struct timeval tv;

    tv.tv_sec = ms / 1000;
    tv.tv_usec = (ms % 1000) * 1000;

    return select(0, NULL, NULL, NULL, &tv);
}

int main() {
    int fd;
    int ret;
    struct sockaddr_in server_addr;
    char buf[10]="123456789";
    int yes;

    fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0) {
        perror("create socket failed");
        return -1;
    }

    yes = 0;
    setsockopt(fd, IPPROTO_TCP, TCP_NODELAY, &yes, sizeof(yes));

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_addr.sin_port = htons(8899);

    ret = connect(fd, (sockaddr *)&server_addr, sizeof(server_addr));
    if (ret < 0) {
        perror("connect to server failed");
        return -1;
    }

    // tc qdisc add dev lo root netem delay 25ms
    // 设置发送网络延时为25ms，一个round-trip就是50ms
    // tcpdump去抓包分析
    for (int i = 0; i < 10; i++) {
        write(fd, buf+(i%10), 1);
        sleep_ms(10);
    }
    close(fd);

    return 0;
}
