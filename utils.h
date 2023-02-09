#ifndef _UTILS_H
#define _UTILS_H

#include <memory>
#include "cpu.h"
#include "global.h"

class raii_interrupt_disable {
public:
    raii_interrupt_disable();

    ~raii_interrupt_disable();
};




#endif /* _UTILS_H */ 