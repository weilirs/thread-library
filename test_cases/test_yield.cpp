#include <iostream>
#include <cstdlib>
#include "thread.h"

using std::cout;
using std::endl;

int g = 0;

mutex mutex1;
cv cv1;

void loop(void *a)
{
    char *id = (char *)a;
    int i;

    mutex1.lock();
    assert_interrupts_enabled();
    cout << "loop called with id " << id << endl;

    for (i = 0; i < 5; i++, g++)
    {
        cout << id << ":\t" << i << "\t" << g << endl;
        mutex1.unlock();
        assert_interrupts_enabled();
        thread::yield();
        assert_interrupts_enabled();
        mutex1.lock();
        assert_interrupts_enabled();
    }
    cout << id << ":\t" << i << "\t" << g << endl;
    mutex1.unlock();
    assert_interrupts_enabled();
}

void yieldtest(void *a)
{
    intptr_t arg = (intptr_t)a;
    thread::yield();
    assert_interrupts_enabled();
    cout << "continue when the ready queue is empty" << endl; // continue when the ready queue is empty after yield

    thread t2((thread_startfunc_t)loop, (void *)"thread 2");
    thread t3((thread_startfunc_t)loop, (void *)"thread 3");

    thread::yield();
    assert_interrupts_enabled();
    cout << "finished" << endl;
}

int main()
{
    cpu::boot(1, (thread_startfunc_t)yieldtest, (void *)100, false, false, 0); // example on the spec when there is only one thread in the ready queue
}
