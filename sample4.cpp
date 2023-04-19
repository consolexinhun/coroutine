#include "libgo/coroutine.h"

#include <iostream>
#include <thread>
#include <cstdio>

#include <chrono>

#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

static const uint16_t port = 43333;

void echo_server();
void client();

int main(int argc, char** argv) {
    go echo_server;
    go client;
    go client;
    go client;
    go client;
    go client;

    co_sched.Start();
    return 0;
}

void echo_server() {
    int accept_fd = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    socklen_t len = sizeof(addr);
    if (bind(accept_fd, (sockaddr*)&addr, len) == -1) {
        fprintf(stderr, "bind error\n");
        exit(1);
    }

    if (listen(accept_fd, 5) == -1) {
        fprintf(stderr, "listen error\n");
        exit(1);
    }

    while (true) {
        int sockfd = accept(accept_fd, (sockaddr*)& addr, &len);
        if (sockfd == -1) {
            if (errno == EAGAIN || errno == EINTR) {
                continue;
            } else {
                fprintf(stderr, "accept error: %s\n", strerror(errno));
                break;
            }
        }

        char buf[1024];
        while (true) {
            int n = read(sockfd, buf, sizeof(buf));
            if (n == -1) {
                if (errno == EAGAIN || errno == EINTR) {
                    continue;
                } else {
                    fprintf(stderr, "read error: %s\n", strerror(errno));
                }
            } else if (n == 0) {
                fprintf(stderr, "read eof\n");
            } else {
                ssize_t write_n = write(sockfd, buf, n);
                (void) write_n;
            }
        }
    }
}

void client() {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (connect(sockfd, (sockaddr*)&addr, sizeof(addr)) == -1) {
        fprintf(stderr, "connect error: %s\n", strerror(errno));
        exit(1);
    }

    char buf[12] = "1234";
    int len = strlen(buf) + 1;  // sizeof(buf)
    ssize_t write_n = write(sockfd, buf, len);
    (void) write_n;
    printf("send [%d] %s\n", len, buf);

    char recv_buf[12];
    while (true) {
        int n = read(sockfd, recv_buf, sizeof(recv_buf));
        if (n == -1) {
            if (errno == EAGAIN || errno == EINTR) {
                continue;
            } else {
                fprintf(stderr, "read error:%s\n", strerror(errno));
            }
        } else if (n == 0) {
            fprintf(stderr, "read eof\n");
        } else {
            printf("recv [%d] : [%s] \n", n, recv_buf);
        }
        break;
    }
    co_sched.Stop();
}
