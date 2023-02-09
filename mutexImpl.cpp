#include "mutexImpl.h"
#include "global.h"
#include "cpu.h"
#include "cpuImpl.h"
using std::shared_ptr;

mutex::impl::impl():isFree(true) {
}

mutex::impl::~impl() {
}

void mutex::impl::lock() {
    if (isFree) {
        isFree = false;
    }
    else if (!readyList.empty()) {
        cpu::impl::switch2nextReady(waitingList);
    }
    else {
        cpu::interrupt_enable_suspend();
    }
    owner = runningTCB;
}

void mutex::impl::unlock() {
    if (owner.expired() || owner.lock() != runningTCB) {
        throw std::runtime_error("Invoking thread does not hold the mutex");
    }
    isFree = true;
    owner.reset();
    if (!waitingList.empty()) {
        readyList.push(waitingList.front());
        waitingList.pop();
        isFree = false;
    }
}
