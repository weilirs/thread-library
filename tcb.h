#ifndef _TCB_H
#define _TCB_H

#include <ucontext.h>
#include "thread.h"
#include <queue>
#include <memory>

class TCB {
private:
    ucontext_t* ucontext_ptr;
    char* stack;

public:
    ucontext_t* getContextPt();
    TCB(thread_startfunc_t body, void* arg);
    ~TCB();

    std::queue<std::shared_ptr<TCB>> waitingList;
};

#endif /* _TCB_H */ 