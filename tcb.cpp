#include "tcb.h"
#include "thread.h"
#include "threadImpl.h"

TCB::TCB(thread_startfunc_t body, void* arg) {
    ucontext_ptr = new ucontext_t();
    stack = new char [STACK_SIZE];
    ucontext_ptr->uc_stack.ss_sp = stack;
    ucontext_ptr->uc_stack.ss_size = STACK_SIZE;
    ucontext_ptr->uc_stack.ss_flags = 0;
    ucontext_ptr->uc_link = nullptr;
    
    makecontext(ucontext_ptr, (void (*)())thread::impl::stub, 2, body, arg);
}

TCB::~TCB() {
    delete[] stack;
    delete ucontext_ptr;
}

ucontext_t* TCB::getContextPt() {
    return ucontext_ptr;
}