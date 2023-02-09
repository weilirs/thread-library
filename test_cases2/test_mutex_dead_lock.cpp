#include <iostream>
#include <cstdlib>
#include "thread.h"
#include "cv.h"
#include "mutex.h"

using std::cout;
using std::endl;

mutex m1;
mutex m2;

void child1(void *a)
{
    cout << "child1 acquiring mutex1 lock" << endl;
    m1.lock();
    assert_interrupts_enabled();
    cout << "child1 acquired mutex1 lock" << endl;
    
    thread::yield();
    assert_interrupts_enabled();

    cout << "child1 acquiring mutex2 lock" << endl;
    m2.lock();
    assert_interrupts_enabled();
    cout << "child1 acquired mutex2 lock" << endl;

    cout << "child1 releasing mutex2 lock" << endl;
    m2.unlock();
    assert_interrupts_enabled();
    cout << "child1 released mutex2 lock" << endl;

    cout << "child1 releasing mutex1 lock" << endl;
    m1.unlock();
    assert_interrupts_enabled();
    cout << "child1 released mutex1 lock" << endl;

    cout << "child1 finish" << endl;
}

void child2(void *a)
{
    cout << "child2 acquiring mutex2 lock" << endl;
    m2.lock();
    assert_interrupts_enabled();
    cout << "child2 acquired mutex2 lock" << endl;

    cout << "child2 acquiring mutex1 lock" << endl;
    m1.lock();
    assert_interrupts_enabled();
    cout << "child2 acquired mutex1 lock" << endl;
    
    cout << "child1 releasing mutex1 lock" << endl;
    m1.unlock();
    assert_interrupts_enabled();
    cout << "child1 released mutex1 lock" << endl;

    cout << "child1 releasing mutex2 lock" << endl;
    m2.unlock();
    assert_interrupts_enabled();
    cout << "child1 released mutex2 lock" << endl;
}

void parent(void *a)
{
    intptr_t arg = (intptr_t) a;
    thread t1 = thread((thread_startfunc_t) child1, (void *) "child thread");
    thread t2 = thread((thread_startfunc_t) child2, (void *) "child thread");
}

int main()
{
    cpu::boot(1, (thread_startfunc_t) parent, (void *) 100, false, false, 0);
}