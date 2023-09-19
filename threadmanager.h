#ifndef THREADMANAGER_H
#define THREADMANAGER_H

#include <thread>
#include "network.h"

class ThreadManager
{
public:
    ThreadManager(Network *nw);

    ~ThreadManager();
private:
    Network *nw;
    std::thread *training_loop;
};

#endif // THREADMANAGER_H
