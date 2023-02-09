#ifndef _CVIMPL_H
#define _CVIMPL_H

#include "cv.h"
#include "tcb.h"
#include <queue>
#include <memory>

class cv::impl {
private:
    std::queue<std::shared_ptr<TCB>> waitingList;

public:
    impl();
    ~impl();
    void wait(mutex&);
    void signal();
    void broadcast();
};


#endif /* _THREADIMPL_H */ 