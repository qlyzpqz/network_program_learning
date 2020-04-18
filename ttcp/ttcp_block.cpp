#include "common.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <vector>
#include <sys/time.h>
using namespace std;

int read_n(int fd, char *buf, int buf_size) {
    int count = 0;
    int ret;

    while (count < buf_size) {
        ret = read(fd, buf+count, buf_size-count);
        if (ret > 0) {
            count += ret;
        } else if (ret == 0) { // EOF
            break;
        } else if (errno == EAGAIN) { // INTERRUPTED
            continue;
        } else {
            perror("read_n failed");
            exit(-1);
        }
    }
    return count;
}

int write_n(int fd, const char *buf, int buf_size) {
    int count = 0;
    int ret;
    while (count < buf_size) {
        ret = write(fd, buf+count, buf_size-count);
        if (ret >= 0) {
            count += ret;
        } else if (errno == EAGAIN) {
            continue;
        } else {
            perror("write_n failed");
            exit(-1);
        }
    }
    return count;
}

int transmit(const Option &opt) {
    assert(opt.client == true);
    int fd;
    sockaddr_in addr;
    SessionMessage session_msg;
    PayloadMessage *payload_msg;
    int ret;
    int total_len;
    struct timeval all_begin, all_end, one_begin, one_end;
    int send_bytes = 0;
    int spend_ms;
    vector<int> vec_spend_time;

    fd = socket(AF_INET, SOCK_STREAM, 0);
    resolveOrDie(opt.host.c_str(), opt.port, &addr);

    ret = connect(fd, (sockaddr *)&addr, sizeof(addr));
    if (ret != 0) {
        printf("unable to connect to host: %s", opt.host.c_str());
        return -1;
    }
    printf("connect to server success\n");

    total_len = opt.length + sizeof(int32_t);

    session_msg.number = ntohl(opt.number);
    session_msg.length = ntohl(opt.length);

    ret = write_n(fd, (const char *)&session_msg, sizeof(session_msg));
    assert(ret == sizeof(session_msg));
    printf("send session message, ret=%d\n", ret);

    payload_msg = (PayloadMessage *)malloc(total_len);

    gettimeofday(&all_begin, NULL);
    for (size_t i = 0; i < opt.number; i++) {
        gettimeofday(&one_begin, NULL);
        send_bytes += total_len;
        ret = write_n(fd, (const char *)payload_msg, total_len);
        // printf("send payload message, idx=%d, ret=%d\n", i, ret);
        gettimeofday(&one_end, NULL);
        vec_spend_time.push_back((one_end.tv_sec-one_begin.tv_sec)*1000+(one_end.tv_usec-one_end.tv_usec)/1000);
    }
    gettimeofday(&all_end, NULL);

    spend_ms = (all_end.tv_sec-all_begin.tv_sec)*1000+(all_end.tv_usec-all_begin.tv_usec)/1000;
    printf("total send: %dMB, time: %dms, speed: %.2lfKb/s\n", send_bytes/1024/1024, spend_ms, send_bytes/1024/(spend_ms/1000.0));

    if (payload_msg != NULL) {
        free(payload_msg);
    }
}

int receive(const Option &opt) {
    int listen_fd, fd;
    struct sockaddr_in addr;
    socklen_t socklen;
    struct SessionMessage session_msg;
    struct PayloadMessage payload_msg;
    int total_len;
    char *buf;
    int ret;

    assert(opt.server == true);

    listen_fd = socket(AF_INET, SOCK_STREAM, 0);

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr("0.0.0.0");
    addr.sin_port = htons(opt.port);

    ret = bind(listen_fd, (sockaddr *)&addr, sizeof(addr));
    if (ret != 0) {
        perror("bind failed");
        return -1;
    }

    ret = listen(listen_fd, 1024);
    if (ret != 0) {
        perror("listen failed");
        return -1;
    }

    fd = accept(listen_fd, (sockaddr *)&addr, &socklen);
    if (ret != 0) {
        perror("accept failed");
        return -1;
    }

    ret = read_n(fd, (char *)&session_msg, sizeof(session_msg));
    assert(ret == sizeof(session_msg));
    session_msg.number = ntohl(session_msg.number);
    session_msg.length = ntohl(session_msg.length);
    printf("receive session msesage, number=%d, length=%d\n", session_msg.number, session_msg.length);

    total_len = session_msg.length + sizeof(int32_t);
    buf = (char *)malloc(total_len);

    for (int i = 0; i < session_msg.number; i++) {
        ret = read_n(fd, buf, total_len);
        assert(ret == total_len);
    }
}

int main(int argc, char **argv) {
    Option option;

    if (parseFlag(argc, argv, &option) == 0) {
        if (option.client) {
            transmit(option);
        }
        if (option.server) {
            receive(option);
        }
    }
}
