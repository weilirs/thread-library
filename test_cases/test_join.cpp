#include <iostream>
#include <cstdlib>
#include "thread.h"

using std::cout;
using std::endl;

int g = 0;

mutex mutex1;
// cv cv1;

void basic(void *a)
{
    char *id = (char *)a;
    int i;

    cout << "loop called with id " << id << endl;

    for (i = 0; i < 5; i++, g++)
    {
        cout << id << ":\t" << i << "\t" << g << endl;
    }
    cout << id << ":\t" << i << "\t" << g << endl;
}

void first(void *a)
{
    cout << "first finished" << endl;
}

void second(void *a)
{
    thread *arg = (thread *)a;
    arg->join();
    assert_interrupts_enabled();
    cout << "second finished" << endl;
}

void join1(void *a)
{
    intptr_t arg = (intptr_t)a;

    thread t1((thread_startfunc_t)basic, (void *)"child thread");
    assert_interrupts_enabled();
    t1.join();
    assert_interrupts_enabled();

    // BUG FIXED thread continues executing if join on a finished thread
    thread t2((thread_startfunc_t)basic, (void *)"child thread");
    assert_interrupts_enabled();
    thread t3((thread_startfunc_t)basic, (void *)"child thread");
    assert_interrupts_enabled();
    t3.join();
    assert_interrupts_enabled();
    t2.join();
    assert_interrupts_enabled();

    thread t5((thread_startfunc_t)first, (void *)"child thread");
    assert_interrupts_enabled();
    thread t4((thread_startfunc_t)second, (void *)(&t5));
    assert_interrupts_enabled();
    t5.join();
    assert_interrupts_enabled();
}

void join3(void *a)
{
    intptr_t arg = (intptr_t)a;
}

int main()
{
    cpu::boot(1, (thread_startfunc_t)join1, (void *)100, false, false, 0); // basic join
}