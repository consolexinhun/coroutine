#include "libgo/coroutine.h"

#include <iostream>
#include <thread>
#include <cstdio>

int main(int argc, char** argv) {
    co::Scheduler* sched = co::Scheduler::Create();

    std::thread t([sched]{
        sched->Start(1);
    });

    go co_scheduler(sched)[] {
        printf("run in my scheduler\n");
    };

    co_sleep(100);

    t.join();
    return 0;
}
