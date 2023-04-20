#include "libgo/coroutine.h"

#include <iostream>
#include <thread>
#include <cstdio>
#include <mutex>
#include <chrono>

struct MySQLConnection {
    int query(const char* sqlStatement) { return 0; }
    bool isAlive() { return true; }
};

bool checkAlice(MySQLConnection* c) {
    return c->isAlive();
}

int main(int argc, char** argv) {
    co::ConnectionPool<MySQLConnection> pool([] {
        return new MySQLConnection;
    }, NULL, 100, 20);

    pool.Reserve(10);

    auto connPtr = pool.Get();
    if (connPtr) {
        printf("get connection ok\n");
    }

    connPtr.reset();

    connPtr = pool.Get(&checkAlice, &checkAlice);
    if (connPtr) {
        printf("get connection ok \n");
    }

    connPtr.reset();

    go [&] {
        connPtr = pool.Get(std::chrono::milliseconds(100));
        if (connPtr) {
            printf("get connection ok \n");
        }

        co_sched.Stop();
    };

    co_sched.Start();
    return 0;
}
