#include "cpuImpl.h"
#include "cpu.h"
#include "global.h"
#include "utils.h"

using std::shared_ptr;

void cpu::impl::interruptHandler() {
    raii_interrupt_disable disabler;
    if (readyList.size() > 0) {
        cpu::impl::switch2nextReady(readyList);
    }
}

// store the original TCB at origQueue
void cpu::impl::switch2nextReady(std::queue<std::shared_ptr<TCB>>& origQueue) {
    origQueue.push(runningTCB);
    runningTCB = readyList.front();
    readyList.pop();
    swapcontext(origQueue.back().get()->getContextPt(), runningTCB.get()->getContextPt());
}