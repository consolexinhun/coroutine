#include "libgo/coroutine.h"

#include <iostream>
#include <thread>
#include <cstdio>
#include <mutex>
#include <chrono>
#include <atomic>

using namespace std;
using namespace std::chrono;

const int nWork = 100;

// 大计算量的函数
int c = 0;
std::atomic<int> done{0};
void foo()
{
    int v = (int)rand();
    for (int i = 1; i < 20000000; ++i) {
        v *= i;
    }
    c += v;

    if (++done == nWork * 2)
        co_sched.Stop();
}

int main()
{
    // 编写cpu密集型程序时, 可以延长协程执行的超时判断阈值, 避免频繁的worksteal产生
    co_opt.cycle_timeout_us = 1000 * 1000;

    // 普通的for循环做法
    auto start = system_clock::now();
    for (int i = 0; i < nWork; ++i)
        foo();
    auto end = system_clock::now();
    cout << "for-loop, cost ";
    cout << duration_cast<milliseconds>(end - start).count() << "ms" << endl;

    // 使用libgo做并行计算
    start = system_clock::now();
    for (int i = 0; i < nWork; ++i)
        go foo;

    // 创建8个线程去并行执行所有协程 (由worksteal算法自动做负载均衡)
    co_sched.Start(1);

    end = system_clock::now();
    cout << "go with coroutine, cost ";
    cout << duration_cast<milliseconds>(end - start).count() << "ms" << endl;
	cout << "result zero:" << c * 0 << endl;
    return 0;
}

// constexpr int nWork = 100;

// int c = 0;
// std::atomic<int> done {0};

// void foo() {
//     int v = rand();
//     for (int i = 1; i < 2 * 1e7; i++) {
//         v *= i;
//     }
//     c += v;
//     if (++done == nWork * 2) {
//         co_sched.Stop();
//     }
// }

// int main(int argc, char** argv) {
//     co_opt.cycle_timeout_us = 1000* 1000;

//     auto start = std::chrono::system_clock::now();
//     for (int i = 0; i < nWork; i++) {
//         foo();
//     }

//     auto end = std::chrono::system_clock::now();

//     std::cout << "for-loop cost:";
//     std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << "ms" << std::endl;

//     start = std::chrono::system_clock::now();
//     for (int i = 0; i < nWork; i++) {
//         go foo;
//     }

//     co_sched.Start(16);
//     end = std::chrono::system_clock::now();

//     std::cout << "go with coroutine cost";
//     std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << "ms" << std::endl;
//     // std::cout << "result zero: " << c*0 << std::endl;
//     return 0;
// }
