#include "common.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>

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
            perror("write_n failed");
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
        if (ret > 0) {
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

    fd = socket(AF_INET, SOCK_STREAM, 0);
    resolveOrDie(opt.host.c_str(), opt.port, &addr);

    connect(fd, (sockaddr *)&addr, sizeof(addr));
}

int receive(const Option &opt) {
    assert(opt.server == true);
}

int main(int argc, char **argv) {
    Option option;

    if (parseFlag(argc, argv, &option) == 0) {
    }
}
