#include "libgo/coroutine.h"

#include <iostream>
#include <thread>
#include <cstdio>
#include <mutex>
#include <chrono>

using std::cout;
using std::endl;

void foo() {
    auto& dis1 = co_last_defer();
    {
        co_defer_scope {
            cout << "go defer 1" << endl;
        };

        auto& dis2 = co_last_defer();
        bool success = dis2.dismiss();
        cout << "IsOk:" << success << endl;
    }

    auto& dis3 = co_last_defer();
    bool success = dis3.dismiss();
    cout << "Isok:" << !success << endl;
    cout << "Isok:" << (&dis1 == &dis3) << endl;
}
int main(int argc, char** argv) {
    co_defer [&] {
        cout << "defer 3" <<endl;
    };

    co_defer_scope {
        cout << "defer 1" << endl;
        cout << "defer 2" << endl;
    };

    co_defer [] {
        cout << "cancel 1" << endl;
    };

    auto& defer_obj = co_last_defer();
    defer_obj.dismiss();

    co_defer [] {
        cout << "cancel 2" << endl;
    };
    co_last_defer().dismiss();

    go foo;

    std::thread([]{
        co_sleep(200);
        co_sched.Stop();
    }).detach();

    co_sched.Start();
    return 0;
}
