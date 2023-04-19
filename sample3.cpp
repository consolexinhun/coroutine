#include "libgo/coroutine.h"

#include <iostream>
#include <thread>
#include <cstdio>

#include <chrono>

int main(int argc, char** argv) {
    /* 阻塞 channel */
    co_chan<int> ch_0;

    go [=] {
        ch_0 << 1;
    };

    go [=] {
        int i;
        ch_0 >> i;
        printf("i = %d\n", i);
    };

    /* 缓冲 channel */
    co_chan<std::shared_ptr<int>> ch_1(1);

    go [=] {
        std::shared_ptr<int> p1 (new int(1));
        ch_1 << p1;
        ch_1 << p1;  /* 再次写入就会阻塞当前协程 */
    };

    go [=] {
        std::shared_ptr<int> p2;
        ch_1 >> p2;
        ch_1 >> p2;
        printf("*ptr = %d\n", *p2);
    };

    /* 超时等待 */
    co_chan<int> ch_2;
    go [=] {
        /* TryPush TryPop 非阻塞调用 */
        int val = 0;
        bool isSucess = ch_2.TryPop(val);


        /* TimedPop 和 TimedPush 超时等待 */
        isSucess = ch_2.TimedPush(1, std::chrono::milliseconds(10));
        (void)isSucess;
    };

    /* 
    Channel 线程安全
    Channel 不用关心是属于哪个调度器的
    Channel 可以用于原生线程中    
     */

    std::thread ([]{
        co_sleep(200);
        co_sched.Stop();
    }).detach();

    co_sched.Start();
    return 0;
}
