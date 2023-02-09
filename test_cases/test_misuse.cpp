#include <iostream>
#include <cstdlib>
#include "thread.h"

using std::cout;
using std::endl;

mutex mutex1;
cv cv1;

void temp()
{
    mutex1.lock();
    assert_interrupts_enabled();
    cv1.signal();
    assert_interrupts_enabled();
    mutex1.unlock();
    assert_interrupts_enabled();
}

void releaseNotHolding1()
{
    try
    {
        mutex1.unlock();
    }
    catch (std::runtime_error)
    {
        cout << "realease wrong lock" << endl;
        assert_interrupts_enabled();
    }

    try
    {
        mutex1.lock();
        mutex1.unlock();
        mutex1.unlock();
    }
    catch (std::runtime_error)
    {
        cout << "realease wrong lock 2" << endl;
        assert_interrupts_enabled();
    }

    try
    {
        cv1.wait(mutex1);
    }
    catch (std::runtime_error)
    {
        cout << "realease wrong lock 3" << endl;
        assert_interrupts_enabled();
    }

    try
    {
        mutex1.lock();
        thread t2((thread_startfunc_t)temp, (void *)"thread 2");
        assert_interrupts_enabled();
        // TODO: DEBUG
        cv1.wait(mutex1);
        cout << "hereh" << endl;
        cv1.wait(mutex1);
    }
    catch (std::runtime_error)
    {
        cout << "realease wrong lock 4" << endl;
    }
}

int main()
{
    cpu::boot(1, (thread_startfunc_t)releaseNotHolding1, (void *)100, false, false, 0); //
}