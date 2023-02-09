#include <iostream>
#include <cstdlib>
#include "thread.h"
#include "cv.h"

using std::cout;
using std::endl;

mutex mutex1;
cv cv1; 

int g = 0;

void deadlock(void *a)
{
    char * arg = (char *) a;

    mutex1.lock();
    assert_interrupts_enabled();
    cout << "child called with arg" << arg << endl;
    mutex1.unlock();
    assert_interrupts_enabled();

    g = 1;
    cout << "child waking up" << arg << endl;
    cv1.signal();
    assert_interrupts_enabled();
    cout << "here" << endl;

    thread::yield();
    assert_interrupts_enabled();

    mutex1.lock();
    assert_interrupts_enabled();
    cv1.signal();
    assert_interrupts_enabled();
    mutex1.unlock();
    assert_interrupts_enabled();
    thread::yield();
    assert_interrupts_enabled();
    mutex1.lock();
    assert_interrupts_enabled();
}

void parent(void *a)
{
    intptr_t arg = (intptr_t) a;

    thread::yield;
    assert_interrupts_enabled();
    mutex1.lock();
    assert_interrupts_enabled();
    cout << "parent called with arg " << arg << endl;
    mutex1.unlock();
    assert_interrupts_enabled();

    thread t1 ( (thread_startfunc_t) deadlock, (void *) "child thread");
    assert_interrupts_enabled();

    mutex1.lock();
    assert_interrupts_enabled();
    while (g==0) {
        cv1.wait(mutex1);
        assert_interrupts_enabled();
        cout << "parent awaked" << endl;
    }
    mutex1.unlock();
    assert_interrupts_enabled();

    mutex1.lock();
    assert_interrupts_enabled();
    cv1.wait(mutex1);
    assert_interrupts_enabled();
    cout << "parent awaked again " << endl;

    mutex1.unlock();
    assert_interrupts_enabled();
}

int main()
{
    cpu::boot(1, (thread_startfunc_t) parent, (void *) 100, false, false, 0);
}