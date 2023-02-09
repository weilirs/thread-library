#include <iostream>
#include <cstdlib>
#include "cpu.h"

using std::cout;
using std::endl;

void parent(void *a)
{
    intptr_t arg = (intptr_t) a;
    cout << "parent starts" << endl;
    thread::yield();
    assert_interrupts_enabled();
    cout << "parent ends" << endl;
}

int main()
{
    cpu::boot(1, (thread_startfunc_t) parent, (void *) 100, false, false, 0);
}