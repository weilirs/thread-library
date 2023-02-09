#include <iostream>
#include <cstdlib>
#include "thread.h"

using std::cout;
using std::endl;

mutex mutex1;
mutex mutex2;

void unlock(void *a)
{
    cout << "test unlock" << endl;
    // mutex1.unlock();
}

void parent(void *a)
{
    intptr_t arg = (intptr_t)a;

    mutex1.lock();
    mutex2.lock();
    cout << "parent called with arg " << arg << endl;
    mutex1.unlock();
    mutex2.unlock();

    thread t1((thread_startfunc_t)unlock, (void *)"child thread");
}

int main()
{
    cpu::boot(1, (thread_startfunc_t)parent, (void *)100, true, true, 0);
}