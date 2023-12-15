#ifndef TCPMAINWINDOW_H
#define TCPMAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QTimer>
#include <QTime>


#include "renewalmainwindow.h"

class TcpMainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit TcpMainWindow(QWidget *parent = nullptr);
    ~TcpMainWindow();

    void initUi();
    void renewalBusiness(FileMessage *res);

signals:
    void unpackSignal(QByteArray);

public slots:
    void myRead();
    void heartCheckSlot();
    void unpackSlot(QByteArray);

private:
    QTcpSocket *m_client;
    int m_heartCheckTimes;
    QTimer *m_checkTimer;
    QTimer *m_qsendTimer;
    RenewalMainWindow *m_renewal;

};

#endif // TCPMAINWINDOW_H
