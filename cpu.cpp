#include "cpu.h"
#include "thread.h"
#include "global.h"
#include "cpuImpl.h"
#include "threadImpl.h"

void cpu::init(thread_startfunc_t body, void * arg) {
    cpu::interrupt_vector_table[cpu::TIMER] = cpu::impl::interruptHandler;
    if (body) {
        try {
            thread::impl(body, arg);
        }
        catch (const std::bad_alloc &e) {
            cpu::interrupt_enable();
            throw e;
        }
        if (!readyList.empty()) {
            runningTCB = readyList.front();
            readyList.pop();
            setcontext(runningTCB.get()->getContextPt());
        }
    }    
}