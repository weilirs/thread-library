#include <iostream>
#include <cstdlib>
#include "thread.h"
#include "cv.h"
#include "mutex.h"

using std::cout;
using std::endl;

mutex mutex1;

void child1(void *a)
{
    mutex1.lock();
    assert_interrupts_enabled();
    cout << "child1 finish" << endl;
}

void child2(void *a)
{
    mutex1.lock();
    assert_interrupts_enabled();
    cout << "child2 finish" << endl;
}

void child3(void *a)
{
    try {
        mutex1.unlock();
    }
    catch (std::runtime_error &e) {
        assert_interrupts_enabled();
        cout << "child3 error" << endl;
    }
}

void parent(void *a)
{
    intptr_t arg = (intptr_t) a;

    thread t1 = thread((thread_startfunc_t) child1, (void *) "child thread");
    thread t2 = thread((thread_startfunc_t) child2, (void *) "child thread");
    thread t3 = thread((thread_startfunc_t) child3, (void *) "child thread");

}

int main()
{
    cpu::boot(1, (thread_startfunc_t) parent, (void *) 100, false, false, 0);
}