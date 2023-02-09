#include "cvImpl.h"
#include "cpu.h"
#include "cpuImpl.h"
#include "global.h"
#include "mutex.h"
#include "mutexImpl.h"

using std::shared_ptr;

cv::impl::impl() {
}

cv::impl::~impl() {
}

void cv::impl::wait(mutex& mt) {
    // mutex not held error handling in mutex class
    mt.impl_ptr->unlock();
    if (!readyList.empty()) {
        cpu::impl::switch2nextReady(waitingList);
    } else {
        cpu::interrupt_enable_suspend();
    }
    mt.impl_ptr->lock();
}

void cv::impl::signal() {
    if(!waitingList.empty()) {
        readyList.push(waitingList.front());
        waitingList.pop();
    }
}

void cv::impl::broadcast() {
    while(!waitingList.empty()){
        readyList.push(waitingList.front());
        waitingList.pop();
    }
}
