#ifndef MYCLIENT_H
#define MYCLIENT_H
#include <QObject>
#include <QTcpSocket>
#include <QTimer>
#include "MyProto.h"

class Myclient:public QObject
{
    Q_OBJECT
public:
    static Myclient& getServer();
    void Init(int port, QString addr,void * p);
    int Run();
    void myWrite(char* p,int len);
signals:
    void unpackSignal(QByteArray);
    void realtimeScreen(QByteArray);
    void history(QByteArray);
    void log(QByteArray);
    void permisson(QByteArray);
    void renewal(QByteArray);
    void versionManager(QByteArray);
private slots:
    void myRead();  // 收包槽函数

   void unpackSlot(QByteArray buf);

    void heartCheckSlot(); // 定时发心跳包的槽
private:
    ~Myclient();
    Myclient();
    Myclient(Myclient&) = delete;
    Myclient(Myclient&&) = delete;
    Myclient& operator=(Myclient&) = delete;
    Myclient& operator=(Myclient&&) = delete;

private:
    QTcpSocket *m_client;
    int m_heartCheckTimes;
    QTimer *m_checkTimer;
    int m_port;
    QString m_addr;
    void * m_ptr;
};

#endif // MYCLIENT_H
