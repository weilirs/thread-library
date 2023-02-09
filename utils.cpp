#include "global.h"
#include "utils.h"
#include <assert.h>

raii_interrupt_disable::raii_interrupt_disable() {
    cpu::interrupt_disable();
    while (!finishList.empty()) {
        assert(finishList.front().use_count()==1);
        finishList.pop();
    }
}

raii_interrupt_disable::~raii_interrupt_disable() {
    while (!finishList.empty()) {
        // ensure invariant: any shared_pointer use_count==1 any time
        // check no remaining shared_pointer
        assert(finishList.front().use_count()==1);
        finishList.pop();
    }
    cpu::interrupt_enable();
}