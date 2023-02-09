#ifndef _GLOBAL_H
#define _GLOBAL_H

#include <queue>
#include <unordered_map>
#include <memory>
#include "tcb.h"

// .h file "declaring" extern global variables

extern std::queue<std::shared_ptr<TCB>> readyList;
extern std::queue<std::shared_ptr<TCB>> finishList;
extern std::shared_ptr<TCB> runningTCB;

#endif /* _GLOBAL_H */