#include <iostream>
#include <cstdlib>
#include "thread.h"
#include "cv.h"

using std::cout;
using std::endl;

thread *target;

void targetChild(void *a)
{
    cout << "targetChild starts" << endl;
    thread::yield();
    assert_interrupts_enabled();
    cout << "targetChild ends" << endl;
}

void joinChild(void *a)
{
    char *arg = (char *)a;
    cout << arg << " starts" << endl;
    target->join();
    assert_interrupts_enabled();
    cout << arg << " ends" << endl;
}

void parent(void *a)
{
    intptr_t arg = (intptr_t)a;
    cout << "parent starts" << endl;
    target = new thread((thread_startfunc_t)targetChild, (void *)"child thread");
    assert_interrupts_enabled();
    thread t1 = thread((thread_startfunc_t)joinChild, (void *)"child thread 1");
    assert_interrupts_enabled();
    thread t2 = thread((thread_startfunc_t)joinChild, (void *)"child thread 2");
    assert_interrupts_enabled();
    thread t3 = thread((thread_startfunc_t)joinChild, (void *)"child thread 3");
    assert_interrupts_enabled();
    thread::yield();
    assert_interrupts_enabled();
    cout << "parent ends" << endl;
}

int main()
{
    cpu::boot(1, (thread_startfunc_t)parent, (void *)100, false, false, 0);
}