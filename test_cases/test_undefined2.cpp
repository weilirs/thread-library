#include <iostream>
#include <cstdlib>
#include "thread.h"

using std::cout;
using std::endl;

mutex mutex1;

void temp(thread *a)
{
    a->join();
    assert_interrupts_enabled();
}

void parent(void *a)
{

    thread t1((thread_startfunc_t)temp, (void *)"child thread");
    assert_interrupts_enabled();
    temp(&t1);
}

int main()
{
    cpu::boot(1, (thread_startfunc_t)parent, (void *)100, true, true, 0);
}