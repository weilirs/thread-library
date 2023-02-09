#include "mutex.h"
#include "mutexImpl.h"
#include "utils.h"

mutex::mutex() {
    impl_ptr = new mutex::impl();
}

mutex::~mutex() {
    delete impl_ptr;
}

void mutex::lock() {
    raii_interrupt_disable disabler;
    impl_ptr->lock();
}

void mutex::unlock() {
    raii_interrupt_disable disabler;
    impl_ptr->unlock();
}