#include <iostream>

#include "thread.h"

using std::cout;
using std::endl;

mutex mutex1;
cv cv1;
int g = 0;

void child1() {
    cout << "child1 started" << endl;
    mutex1.lock();
    g = 1;
    cv1.broadcast();
    assert_interrupts_enabled();
    mutex1.unlock();
    cout << "child1 finished" << endl;
}

void child2() {
    cout << "child2 started" << endl;
    mutex1.lock();
    assert_interrupts_enabled();
    cout << "child2 wait" << endl;
    while (g==0) {
        cv1.wait(mutex1);
        assert_interrupts_enabled();
    }
    mutex1.unlock();
    assert_interrupts_enabled();
    cout << "child2 finished" << endl;
}

void parent(void *a) {

    thread t2((thread_startfunc_t)child2, (void *)100);
    thread t1((thread_startfunc_t)child1, (void *)100);

    mutex1.lock();
    assert_interrupts_enabled();
    cout << "parent wait" << endl;
    while (g==0) {
        cv1.wait(mutex1);
        assert_interrupts_enabled();
    }
    cout << "parent finished" << endl;
    mutex1.unlock();
}

int main() {
    cpu::boot(1, (thread_startfunc_t)parent, (void *)100, false, false, 0);
}