#pragma once
#include "Task.h"
class Heart :
    public Task
{
    virtual  ~Heart();
    virtual int mySend(vector<char>& data, size_t comm_fd);
};

