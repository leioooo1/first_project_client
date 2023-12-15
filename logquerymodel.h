#ifndef LOGQUERYMODEL_H
#define LOGQUERYMODEL_H

#include <QMainWindow>
#include <QStandardItemModel>
#include <QTcpSocket>

#include "myclient.h"

namespace Ui {
class logQueryModel;
}

class logQueryModel : public QMainWindow
{
    Q_OBJECT

public:
    explicit logQueryModel(QWidget *parent = 0);
    ~logQueryModel();
    friend class LogBusiness;
public slots:
    int myTaskC(QByteArray data);
private slots:
    void on_search_log_clicked();

private:
    Ui::logQueryModel *ui;
    QStandardItemModel *m_model;
    searchLogReq m_searchLogReq;
    QList<searchLogRes> m_logList;
};

#endif // LOGQUERYMODEL_H
