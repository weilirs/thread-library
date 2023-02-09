#include <iostream>
#include <cstdlib>
#include "thread.h"

using std::cout;
using std::endl;

mutex mutex1;

void deadlock(void *a)
{
    mutex1.lock();
    assert_interrupts_enabled();
    cout << "test deadlock 1" << endl;
    mutex1.lock();
    assert_interrupts_enabled();
    cout << "test deadlock 2" << endl;
    mutex1.unlock();
    assert_interrupts_enabled();
}

void parent(void *a)
{
    intptr_t arg = (intptr_t)a;

    mutex1.lock();
    assert_interrupts_enabled();
    cout << "parent called with arg " << arg << endl;
    mutex1.unlock();
    assert_interrupts_enabled();

    thread t1((thread_startfunc_t)deadlock, (void *)"child thread");
    assert_interrupts_enabled();
}

int main()
{
    cpu::boot(1, (thread_startfunc_t)parent, (void *)100, true, true, 0);
}