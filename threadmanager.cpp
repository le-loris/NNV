#include "threadmanager.h"

ThreadManager::ThreadManager(Network *nw)
{
    this->nw = nw;
    this->training_loop = new std::thread(&Network::train, this->nw);
}

ThreadManager::~ThreadManager(){
    this->nw->stopThread();


    if(this->training_loop->joinable()) this->training_loop->join();
}
