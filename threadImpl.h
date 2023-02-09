#ifndef _THREADIMPL_H
#define _THREADIMPL_H

#include "thread.h"
#include "ucontext.h"
#include "tcb.h"
#include <memory>

class thread::impl {
private:
    std::weak_ptr<TCB> TCB_wp;

public:
    impl(thread_startfunc_t, void*);
    ~impl();
    static void yield();
    void join();
    static void stub(thread_startfunc_t, void*);
    static void exit();
};


#endif /* _THREADIMPL_H */ 