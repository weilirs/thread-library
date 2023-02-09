#include <iostream>
#include <cstdlib>
#include "thread.h"
#include "cv.h"
#include "mutex.h"
#include <string>
#include "cpu.h"

using std::cout;
using std::endl;

mutex mutex1;

void child(void *a)
{
    intptr_t arg = (intptr_t) a;
    cout << arg << " starts" << endl;
    for (int i = 0; i < 10000; ++i) {
        assert_interrupts_enabled();
    }
    assert_interrupts_enabled();
    cout << arg << " ends" << endl;
}

void parent(void *a)
{
    intptr_t arg = (intptr_t) a;
    for (int i = 0; i < 20; ++i) {
        thread t = thread((thread_startfunc_t) child, (void *) i);
    }
    cout << "all threads added" << endl;
    assert_interrupts_enabled();
}

int main()
{
    cpu::boot(1, (thread_startfunc_t) parent, (void *) 100, false, false, 0);
}