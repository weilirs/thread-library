#ifndef _MUTEXIMPL_H
#define _MUTEXIMPL_H

#include "mutex.h"
#include "tcb.h"
#include <queue>
#include <memory>

class mutex::impl {
private:
    std::queue<std::shared_ptr<TCB>> waitingList;
    std::weak_ptr<TCB> owner;
    bool isFree;

public:
    impl();
    ~impl();
    void lock();
    void unlock();
};

#endif /* _MUTEXIMPL_H */