#include <iostream>
#include <cstdlib>
#include "thread.h"
#include "cv.h"

using std::cout;
using std::endl;

thread* t;

void deadlock(void *a)
{
    cout << "do nothing" << endl;
    t->join();
    assert_interrupts_enabled();
    cout << "here" << endl;
}

void parent(void *a)
{
    intptr_t arg = (intptr_t) a;

    t = new thread((thread_startfunc_t) deadlock, (void *) "child thread");
    assert_interrupts_enabled();

    thread::yield();
    assert_interrupts_enabled();
    cout << "right here" << endl;
    // t->join();
    // cout << "end here" << endl;
}

int main()
{
    cpu::boot(1, (thread_startfunc_t) parent, (void *) 100, false, false, 0);
}