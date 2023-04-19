#include "libgo/coroutine.h"

#include <iostream>
#include <thread>
#include <cstdio>

int main(int argc, char** argv) {
    go []{
        printf("1\n");
        co_yield;
        printf("2\n");
    };

    go []{
        printf("3\n");
        co_yield;
        printf("4\n");
        // co_sched.Stop();
    };

    co_sched.Start();
    return 0;
}
