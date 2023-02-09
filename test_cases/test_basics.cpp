#include <iostream>
#include <cstdlib>
#include "thread.h"

using std::cout;
using std::endl;

void loop(void *a)
{
    char *id = (char *)a;
    int i;

    cout << "loop called with id " << id << endl;

    for (i = 0; i < 5; i++)
    {
        cout << id << ":\t" << i << endl;
        thread::yield();
        assert_interrupts_enabled();
    }
    cout << id << ":\t" << i << endl;
}

void temp(void *a)
{
    thread t4((thread_startfunc_t)parent, (void *)"child thread4");
    assert_interrupts_enabled();
}
void parent(void *a)
{
    intptr_t arg = (intptr_t)a;

    cout << "parent called with arg " << arg << endl;

    thread t1((thread_startfunc_t)loop, (void *)"child thread1");
    assert_interrupts_enabled();
    cout << "t1 created" << endl;
    thread t2((thread_startfunc_t)loop, (void *)"child thread2");
    assert_interrupts_enabled();
    cout << "t2 created" << endl;

    loop((void *)"parent thread");

    thread t3((thread_startfunc_t)temp, (void *)"child thread3");
    assert_interrupts_enabled();
}

int main()
{
    cpu::boot(1, (thread_startfunc_t)parent, (void *)100, false, false, 0);
}
