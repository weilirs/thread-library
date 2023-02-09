#include <iostream>
#include <cstdlib>
#include "thread.h"
#include "cv.h"

using std::cout;
using std::endl;

thread* t1;
thread* t2; 

void child1(void* a) {
    cout << "child 1 starts" << endl;
    t2->join();
    assert_interrupts_enabled();
    cout << "child 1 ends" << endl;
}

void child2(void* a) {
    cout << "child 2 starts" << endl;
    t1->join();
    assert_interrupts_enabled();
    cout << "child 2 ends" << endl;
}

void parent(void *a)
{
    intptr_t arg = (intptr_t) a;

    t1 = new thread((thread_startfunc_t) child1, (void *) "child thread");
    assert_interrupts_enabled();
    t2 = new thread((thread_startfunc_t) child2, (void *) "child thread");
    assert_interrupts_enabled();
    cout << "parent ends" << endl;

}

int main()
{
    cpu::boot(1, (thread_startfunc_t) parent, (void *) 100, false, false, 0);
}