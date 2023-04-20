#include "libgo/coroutine.h"

#include <iostream>
#include <thread>
#include <cstdio>
#include <mutex>
#include <chrono>

using std::cout;
using std::endl;


struct A {
    A() { cout << "A construct" << endl; }
    ~A() { cout << "A destruct " << endl; }
    int i = 0;
};

void foo() {
    A& a = co_cls(A);
    cout << "foo->" << a.i << endl;
    a.i++;
}

void foo3() {
    for (int i = 0; i < 3; i++) foo();
}

struct B {
    static co_cls_ref(int) a;
};

co_cls_ref(int) B::a = co_cls(int);

void bar() {
    B::a += 10;
    cout << "bar->" << B::a <<endl;
}

auto gCls = co_cls(int);

void car() {
    cout << "car->" <<gCls << endl;
    gCls++;
}

void oneLine() {
    int &a = co_cls(int, 0);
    int &b = co_cls(int, 10);
    int &c = co_cls(int, 20);
    cout << "oneLine->" << "a=" << a << "b=" << b << "c=" << c << endl;;
}

void test() {
    cout << "----------function----------------" << endl;
    go foo3;

    foo();
    foo();

    cout << "-----------------static member----------" << endl;
    go bar;
    bar();
    bar();

    cout << "---------------global------------" << endl;
    go car;
    car();
    car();

    oneLine();

    co_sched.Stop();
}
int main(int argc, char** argv) {
    go test;
    co_sched.Start();
    return 0;
}
