#include <iostream>
#include <memory>
#include <assert.h>
#include "cpu.h"
#include "cpuImpl.h"
#include "threadImpl.h"
#include "global.h"
#include "tcb.h"
#include "utils.h"

using std::shared_ptr;

void thread::impl::yield() {
    if (readyList.size() > 0) {
        cpu::impl::switch2nextReady(readyList);
    }
}

void thread::impl::exit() {
    auto& joinQueue = runningTCB.get()->waitingList;
    while (!joinQueue.empty()) {
        readyList.push(joinQueue.front());
        joinQueue.pop();
    }
    if (readyList.size() > 0) {
        finishList.push(runningTCB);        
        runningTCB = readyList.front();
        readyList.pop();
        swapcontext(finishList.back().get()->getContextPt(), runningTCB.get()->getContextPt());
    }
    else {
        cpu::interrupt_enable_suspend();
    }
}

void thread::impl::stub(thread_startfunc_t body, void *arg) {
    assert_interrupts_disabled();
    assert(runningTCB.use_count()==1);
    while (!finishList.empty()) {
        assert(finishList.front().use_count()==1);
        finishList.pop();
    }
    cpu::interrupt_enable();
    (*body)(arg);
    cpu::interrupt_disable();
    exit();
}

thread::impl::impl(thread_startfunc_t body, void *arg) {
    try {
        shared_ptr<TCB> newTCB(new TCB(body,arg));
        TCB_wp = newTCB;
        readyList.push(newTCB);
    }
    catch (const std::bad_alloc &e) {
        throw e;
    }
}

thread::impl::~impl() {
    // do nothing
}

void thread::impl::join() {
    if (TCB_wp.expired()) {
        return;
    }
    if (readyList.size() > 0) {
        auto& joinQueue = TCB_wp.lock()->waitingList;
        cpu::impl::switch2nextReady(joinQueue);
    }
    else {
        cpu::interrupt_enable_suspend();
    }
}