#include "historyimpltaskc.h"

HistoryImplTaskC::HistoryImplTaskC()
{

}

int HistoryImplTaskC::myTaskC(QByteArray &data, void *pointer)
{
    Head *head = (Head *)(data.data());
    HistoricalQueryMainWindow *historyQuery = (HistoricalQueryMainWindow *)pointer;
    if(head->type == HISTORICAL_QUERY_RES)
    {
        HistoricalRes *history = (HistoricalRes *)(data.data());
        historyQuery->displayResult(history);
    }
}
