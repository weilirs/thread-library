#include "cv.h"
#include "cvImpl.h"
#include "utils.h"

cv::cv() {
    impl_ptr = new cv::impl();
}

cv::~cv() {
    delete impl_ptr;
}

void cv::wait(mutex& mt) {
    raii_interrupt_disable disabler;
    impl_ptr->wait(mt);
}

void cv::signal() {
    raii_interrupt_disable disabler;
    impl_ptr->signal();
}

void cv::broadcast() {
    raii_interrupt_disable disabler;
    impl_ptr->broadcast();
}