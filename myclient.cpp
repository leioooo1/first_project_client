#pragma execution_character_set("utf-8")

#include "myclient.h"
#include <QMutex>
#include "MyProto.h"

#define HEARTTIME 6
QMutex m_mutex;
QByteArray g_allBuffer;
Myclient &Myclient::getServer()
{
    static Myclient m_myClient;
    return m_myClient;
}

void Myclient::Init(int port, QString addr,void * p)
{
    m_port = port;
    m_addr= addr;


    m_ptr = p;

}

int Myclient::Run()
{
    // m_client->close();
    if(m_client){
        delete m_client;
        m_client = new QTcpSocket;
    }
    m_client->connectToHost(m_addr,m_port);
    if (m_client->waitForConnected(1000)){
        connect(m_client,SIGNAL(readyRead()),this,SLOT(myRead()));
        qDebug()<<"连接成功";
         m_mutex.lock();
         m_heartCheckTimes = HEARTTIME;
         m_mutex.unlock();
        // 阈值初始化
        // 心跳检测相关的定时器 和 关联操作
        connect(m_checkTimer,SIGNAL(timeout()),this,SLOT(heartCheckSlot()));
        m_checkTimer->start(5000); // 5秒的间隔定时

    }else{
        connect(m_checkTimer,SIGNAL(timeout()),this,SLOT(heartCheckSlot()));
        m_checkTimer->start(5000); // 5秒的间隔定时
        qDebug()<<"连接失败";
        return -1;
    }

    return 0;
}

void Myclient::myWrite(char *p, int len)
{
    m_client->write(p,len);
}



void Myclient::myRead()
{
    QByteArray buffer = m_client->readAll(); // 收包
     //  qDebug()<<"buffer:"<<buffer;
       g_allBuffer.append(buffer); // 往后追加，存起来

       int len=g_allBuffer.size();
       while (len > 0) {
           // 比较一下数据头的长度
           if((unsigned int)len < sizeof(Head)){ // 和数据头长度对比，小于，说明包头还没收完，继续等下次
               qDebug() << "dd";
               break;
           }
           //通过数据头，取出总长度，类型啥的
           int total_len = ((Head*)g_allBuffer.data())->len;
           // 再次比较封包总长度够不够
           if(len < total_len){ // 全局存的内容长度小于包的总长度，继续等下次
               qDebug() << "MM";
               break;
           }
           // 够了根据类型来进行解包处理
           // 把当前够的这个包，拿去解开并操作
           QByteArray datas = g_allBuffer.left(total_len); // 从左边0下标开始截取指定长度内容
           emit unpackSignal(datas); // 相当于调用了解包函数，通过发信号，可用来指定同步或异步处理

           // 截取剩余长度的内容，继续处理剩下的
           //g_allBuffer = g_allBuffer.right(g_allBuffer.size() - total_len); // 从右边末尾计算截取指定长度
           g_allBuffer = g_allBuffer.mid(total_len); // 从中间指定位置往后截取
           len = g_allBuffer.size(); // 刷新len
       }

}

void Myclient::heartCheckSlot()
{
    HeartCheckReq heat;
    m_client->write((char*)(&heat),heat.head.len); // 发送心跳包
    qDebug() << "heart"<<heat.head.len;
    m_mutex.lock();
    m_heartCheckTimes--; // 递减阈值
    qDebug()<<"心跳阈值为" <<m_heartCheckTimes;

    if(m_heartCheckTimes <= 0)
    {
        // 需要做断线重连操作


        if(m_client){
           m_client->close();
           qDebug() << "close";
        }else {
           qDebug() << "崩了";
        }
        m_client->connectToHost(m_addr,m_port);
          qDebug() << "close";
        if(m_client->waitForConnected(500))
        {
            m_heartCheckTimes =HEARTTIME; // 重连成功，重置阈值
            qDebug()<<"重连成功";
        }else{
            qDebug() << "fail";
        }
    }
    m_mutex.unlock();
}

Myclient::~Myclient()
{
    delete m_client;
    delete m_checkTimer;
}

Myclient::Myclient()
{
    m_checkTimer = new QTimer;
    m_client = new QTcpSocket;// 取消已有的连接
    connect(this,SIGNAL(unpackSignal(QByteArray)),this,SLOT(unpackSlot(QByteArray))); //解包
}
void Myclient::unpackSlot(QByteArray buf)
{
   // qDebug() << ((Head*)buf.data())->type<< "&&&&&&&&&&&&&&&&&&&";
    m_mutex.lock();
    m_heartCheckTimes = HEARTTIME;
    m_mutex.unlock();

    switch (((Head*)buf.data())->type)
    {
    case TypeInfo::MATERIAl_RES:
    case TypeInfo::ATTENDANCE_RES:
    case TypeInfo::WORKSHOPDEVICE_RES:
    {
        emit realtimeScreen(buf);
        break;
    }
    case TypeInfo::HISTORICAL_QUERY_RES:
    {
        emit history(buf);
        break;
    }
    case TypeInfo::SEARCHLOG_RES:
    {
        emit log(buf);
        break;
    }

    case SEARCHPERMISSION_RES:
    case UPDATEPERMISSION_RES:
    {
        emit permisson(buf);
        break;
    }
    case FILE_RES:
    {
        emit renewal(buf);
        break;
    }
    case VER_NUM_REQ:
    {
        emit versionManager(buf);
        break;
    }
    default:
        break;
    }


}
