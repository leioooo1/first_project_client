#ifndef IMPLTASKC_H
#define IMPLTASKC_H

#include "TaskC.h"
#include "realtimescreenscreenmainwindow.h"


class ImplTaskC
{
public:
    ImplTaskC();
    virtual int myTaskC(QByteArray & data, void * pointer);
};

#endif // IMPLTASKC_H
