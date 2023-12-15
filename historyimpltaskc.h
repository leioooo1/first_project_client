#ifndef HISTORYIMPLTASKC_H
#define HISTORYIMPLTASKC_H

#include "TaskC.h"
#include "historicalquerymainwindow.h"

class HistoryImplTaskC
{
public:
    HistoryImplTaskC();
    virtual int myTaskC(QByteArray &data, void *pointer);
};

#endif // HISTORYIMPLTASKC_H
