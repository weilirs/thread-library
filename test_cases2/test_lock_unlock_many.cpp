#include <iostream>
#include <cstdlib>
#include "cpu.h"
#include "thread.h"

using std::cout;
using std::endl;

mutex m;
int g = 0;

void child1(void *a) {
    cout << "child 1 starts" << endl;
    m.lock();
    cout << "child 1 acquired lock" << endl;
    g++;
    cout << "child 1 unlocked, g = " << g << endl;
    m.unlock();
    cout << "child 1 finished" << endl;
}

void child2(void *a) {
    cout << "child 2 starts" << endl;
    m.lock();
    cout << "child 2 acquired lock" << endl;
    g++;
    cout << "child 2 unlocked, g = " << g << endl;
    m.unlock();
    cout << "child 2 finished" << endl;
}

void child3(void *a) {
    cout << "child 3 starts" << endl;
    m.lock();
    cout << "child 3 acquired lock" << endl;
    g++;
    cout << "child 3 unlocked, g = " << g << endl;
    m.unlock();
    cout << "child 3 finished" << endl;
}

void parent(void *a) {
    m.lock();
    cout << "parent acquired lock" << endl;
    thread t1((thread_startfunc_t)child1, (void *)"child");
    thread t2((thread_startfunc_t)child2, (void *)"child");
    thread::yield();
    thread t3((thread_startfunc_t)child3, (void *)"child");
    cout << "parent releasing lock" << endl;
    m.unlock();
    cout << "parent finishing" << endl;
}

int main() {
    cpu::boot(1, (thread_startfunc_t)parent, (void *)100, false, false, 0);
}