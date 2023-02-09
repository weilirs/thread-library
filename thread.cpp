#include "thread.h"
#include "threadImpl.h"
#include "utils.h"

thread::thread(thread_startfunc_t body, void* arg) {
    raii_interrupt_disable disabler;
    impl_ptr = new thread::impl(body, arg);
}

thread::~thread() {
    delete impl_ptr;
}

void thread::yield() {
    raii_interrupt_disable disabler;
    thread::impl::yield();
}

void thread::join() {
    raii_interrupt_disable disabler;
    impl_ptr->join();
}