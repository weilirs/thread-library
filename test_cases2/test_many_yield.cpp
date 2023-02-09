#include <iostream>
#include "thread.h"
using std::cout;
using std::endl;

int g = 0;  // global variable; shared between the two threads

void child(void *a) {
    cout << "child starts" << endl;

    for (int i = 0; i < 1000; ++i) {
        thread::yield();
        assert_interrupts_enabled();
    }
    g = 1;
    cout << "child finish set g =1 " << endl;
}

void parent(void *a) {
    cout << "parent starts" << endl;

    thread t1((thread_startfunc_t)child, (void *)"test message");
    assert_interrupts_enabled();
    
    int counter = 0;
    while (!g) {
        ++counter;
        thread::yield();
        assert_interrupts_enabled();
    }
    cout << "parent finishing with counter  " << counter << endl;
}

int main() {
    cpu::boot(1, (thread_startfunc_t)parent, (void *)100, false, false, 0);
}