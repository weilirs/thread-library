#ifndef _CPUIMPL_H
#define _CPUIMPL_H

#include <queue>
#include <memory>
#include "cpu.h"
#include "tcb.h"

class cpu::impl {
private:
    /* data */
public:
    impl();
    static void interruptHandler();
    static void switch2nextReady(std::queue<std::shared_ptr<TCB>>& origQueue);
};

#endif /* _CPUIMPL_H */ 