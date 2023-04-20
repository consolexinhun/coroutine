#include "libgo/coroutine.h"

#include <iostream>
#include <thread>
#include <cstdio>
#include <mutex>
#include <chrono>

void foo() {
    printf("do some block in co:AsyncCoroutinePool\n");
}

void done() {
    printf("done \n");
}

int calc() {
    return 1024;
}

void callback(int val) {
    printf("calc result : %d\n", val);
}

int main(int argc, char** argv) {
    co::AsyncCoroutinePool* pool = co::AsyncCoroutinePool::Create();

    pool->InitCoroutinePool(1024);

    pool->Start(4, 128);

    auto cbp = new co::AsyncCoroutinePool::CallbackPoint;
    pool->AddCallbackPoint(cbp);

    pool->Post(&foo, &done);

    pool->Post<int>(&calc, &callback);

    while (true) {
        size_t trigger = cbp->Run();
        if (trigger > 0) {
            break;
        }
    }

    co_chan<int> ch(1);
    pool->Post<int>(ch, []{
        return 8;
    }) ;

    int val = 0;
    ch >> val;
    printf("val=%d\n", val);


    co_chan<void> ch2(1);
    pool->Post(ch2, [] {
        printf("void task\n");

    });

    ch2 >> nullptr;
    return 0;
}
