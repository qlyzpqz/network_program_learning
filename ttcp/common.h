#include <unistd.h>
#include <string>
#include <netinet/in.h>

using namespace std;

struct Option {
    bool client;
    bool server;
    int num;
    int length;
    string host;
    unsigned short port;
};

struct SessionMessage {
    int32_t number;
    int32_t length;
};

struct PayloadMessage {
    int32_t length;
    char data[1];
};

int parseFlag(int argc, char **argv, Option *option);

int resolveOrDie(const char *host, unsigned short port, sockaddr_in *addr);
