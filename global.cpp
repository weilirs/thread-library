#include "global.h"

using std::queue;
using std::shared_ptr;

queue<shared_ptr<TCB>> readyList;
queue<shared_ptr<TCB>> finishList;
shared_ptr<TCB> runningTCB;