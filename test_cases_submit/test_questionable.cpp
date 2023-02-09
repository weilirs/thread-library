#include <iostream>
#include <cstdlib>
#include "thread.h"

using std::cout;
using std::endl;

int g = 0;

mutex mutex1;
mutex mutex2;
cv cv1;

void m1()
{
    mutex1.lock();
    assert_interrupts_enabled();
    cv1.broadcast();
    assert_interrupts_enabled();
    mutex1.unlock();
    assert_interrupts_enabled();
}

void m2()
{
    mutex2.lock();
    assert_interrupts_enabled();
    cv1.broadcast();
    assert_interrupts_enabled();
    mutex2.unlock();
    assert_interrupts_enabled();
}

void question(void *a)
{
    cv1.signal();
    assert_interrupts_enabled();

    mutex1.lock();
    assert_interrupts_enabled();
    thread t2((thread_startfunc_t)m1, (void *)"child thread");
    cv1.wait(mutex1);
    mutex1.unlock();
    assert_interrupts_enabled();
    mutex2.lock();
    assert_interrupts_enabled();
    thread t3((thread_startfunc_t)m2, (void *)"child thread");
    cv1.wait(mutex2);
    assert_interrupts_enabled();
    mutex2.unlock();
    assert_interrupts_enabled();

    mutex1.lock();
    assert_interrupts_enabled();
}

int main()
{
    cpu::boot(1, (thread_startfunc_t)question, (void *)100, false, false, 0);
}