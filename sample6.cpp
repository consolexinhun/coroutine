#include "libgo/coroutine.h"

#include <iostream>
#include <thread>
#include <cstdio>
#include <mutex>
#include <chrono>

int main(int argc, char** argv) {
    co_mutex cm;
    cm.lock();

    go [&] {
        for (int i = 0; i < 3; i++) {
            std::lock_guard<co_mutex> lock(cm);
            printf("coroutine 1 lock\n");
        }
    };

    go [&] {
        cm.unlock();
        for (int i = 0; i < 3; i++) {
            std::lock_guard<co_mutex> lock(cm);
            printf("coroutine 2 lock\n");
        }
    };


    /* 读写锁 */

    co_rwmutex m;
    go [&] {
        {
            m.reader().lock();
            m.reader().unlock();

            std::unique_lock<co_rmutex> lock(m.Reader());
        }

        {
            m.writer().lock();
            m.writer().unlock();

            std::unique_lock<co_wmutex> lock(m.Writer());
        }

    };
    std::thread([]{
        co_sleep(200);
        co_sched.Stop();
    }).detach();

    co_sched.Start();
    return 0;
}
