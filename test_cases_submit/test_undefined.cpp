#include <iostream>
#include <cstdlib>
#include "thread.h"

using std::cout;
using std::endl;

mutex mutex1;

void parent(void *a)
{
    thread t3((thread_startfunc_t) nullptr, (void *)"child thread");
    assert_interrupts_enabled();
}

int main()
{
    cpu::boot(1, (thread_startfunc_t)parent, (void *)100, true, true, 0);
}