#include "common.h"
#include <stdlib.h>
#include <stdio.h>
#include <netdb.h>
#include <assert.h>

int parseFlag(int argc, char **argv, Option *option) {
    for (int i = 1; i < argc; i++) {
        if (string(argv[i]) == "-c") {
            option->client = true;
            option->server = false;
        } else if (string(argv[i]) == "-s") {
            option->server = true;
            option->client = false;
        } else if (argv[i] == "-p") {
            if (i + 1 >= argc) {
                perror("unexpected parameter, -p <n>");
                return -1;
            }
            option->port = atoi(argv[i+1]);
            i++;
        } else if (string(argv[i]) == "-n") {
            if (i + 1 >= argc) {
                perror("unexpected parameter, -n <num>");
                return -1;
            }
            option->number = atoi(argv[i+1]);
            i++;
        } else if (string(argv[i]) == "-l") {
            if (i + 1 >= argc) {
                perror("unexpected parameter, -l <length>");
                return -1;
            }
            option->length = atoi(argv[i+1]);
            i++;
        } else if (string(argv[i]) == "-h") {
            if (i + 1 >= argc) {
                perror("unexpected parameter, -h <host>");
                return -1;
            }
            option->host = argv[i+1];
            i++;
        }
    }
    return 0;
}

int resolveOrDie(const char *host, unsigned short port, sockaddr_in *addr)
{
    hostent *pent;
    pent = gethostbyname(host);

    assert(pent->h_addrtype == AF_INET);

    addr->sin_family = AF_INET;
    addr->sin_addr = *(in_addr *)pent->h_addr;
    addr->sin_port = htons(port);
}

