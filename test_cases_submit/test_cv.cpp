#include <iostream>
#include <cstdlib>
#include "thread.h"

using std::cout;
using std::endl;

cv cv1;
cv cv2;
mutex mutex1;

void wait1(void *a)
{
    mutex1.lock();
    assert_interrupts_enabled();
    cv1.wait(mutex1);
    assert_interrupts_enabled();
    mutex1.unlock();
    assert_interrupts_enabled();
}

void wait2(void *a)
{
    mutex1.lock();
    assert_interrupts_enabled();
    cv2.wait(mutex1);
    assert_interrupts_enabled();
    mutex1.unlock();
    assert_interrupts_enabled();
}

void sig(void *a)
{
    mutex1.lock();
    assert_interrupts_enabled();
    cv1.signal();
    assert_interrupts_enabled();
    mutex1.unlock();
    assert_interrupts_enabled();
}

void broad(void *a)
{
    mutex1.lock();
    assert_interrupts_enabled();
    cv1.broadcast();
    assert_interrupts_enabled();
    mutex1.unlock();
    assert_interrupts_enabled();
}

void wait_no_thread(void *a)
{
    mutex1.lock();
    assert_interrupts_enabled();
    cv1.wait(mutex1);
    assert_interrupts_enabled();
}

void signal1()
{

    thread t1((thread_startfunc_t)wait1, (void *)"wait thread");
    assert_interrupts_enabled();
    thread t2((thread_startfunc_t)sig, (void *)"sig thread");
    assert_interrupts_enabled();

    thread t3((thread_startfunc_t)wait1, (void *)"wait thread");
    assert_interrupts_enabled();
    thread t5((thread_startfunc_t)wait1, (void *)"wait thread");
    assert_interrupts_enabled();
    thread t4((thread_startfunc_t)sig, (void *)"sig thread");
    assert_interrupts_enabled();

    thread t6((thread_startfunc_t)wait1, (void *)"wait thread");
    assert_interrupts_enabled();
    thread t8((thread_startfunc_t)wait2, (void *)"wait thread");
    assert_interrupts_enabled();
    thread t7((thread_startfunc_t)sig, (void *)"sig thread");
    assert_interrupts_enabled();

    thread t9((thread_startfunc_t)wait1, (void *)"wait thread");
    assert_interrupts_enabled();
    thread t10((thread_startfunc_t)broad, (void *)"broad thread");
    assert_interrupts_enabled();

    thread t11((thread_startfunc_t)wait1, (void *)"wait thread");
    assert_interrupts_enabled();
    thread t13((thread_startfunc_t)wait1, (void *)"wait thread");
    assert_interrupts_enabled();
    thread t12((thread_startfunc_t)broad, (void *)"broad thread");
    assert_interrupts_enabled();

    thread t14((thread_startfunc_t)wait1, (void *)"wait thread");
    assert_interrupts_enabled();
    thread t16((thread_startfunc_t)wait2, (void *)"wait thread");
    assert_interrupts_enabled();
    thread t15((thread_startfunc_t)broad, (void *)"sig thread");
    assert_interrupts_enabled();
}

int main()
{
    // cpu::boot(1, (thread_startfunc_t) wait_no_thread, (void *) 100, false, false, 0); // wait on the only thread
    cpu::boot(1, (thread_startfunc_t)signal1, (void *)100, false, false, 0); // basic signal, only 1 thread to be signal
}
