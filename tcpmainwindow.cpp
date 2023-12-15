#include "tcpmainwindow.h"
#include <QDebug>
#include <QByteArray>
#include <QMessageBox>
#include <fstream>
#include <iostream>
#include <io.h>
#pragma execution_character_set("utf-8")

QByteArray g_allBuffer;
#define HEART_CHECK_TIMES 60 // 保活30秒，每5秒发一次心跳包，阈值为6

TcpMainWindow::TcpMainWindow(QWidget *parent) : QMainWindow(parent)
{
    m_client = new QTcpSocket(this);
    m_client->connectToHost("192.168.3.200",8888);
    if(m_client->waitForConnected()){
        qDebug() << "conn ok";
        connect(m_client,SIGNAL(readyRead()),this,SLOT(myRead()));
        initUi();//初始化界面

        m_heartCheckTimes = HEART_CHECK_TIMES;
        m_checkTimer = new QTimer(this);
        connect(m_checkTimer,SIGNAL(timeout()),this,SLOT(heartCheckSlot()));
        m_checkTimer->start(5000);
    }else{
        qDebug() << "conn fail" << m_client->errorString();
    }
    connect(this,SIGNAL(unpackSignal(QByteArray)),this,SLOT(unpackSlot(QByteArray)));
}

TcpMainWindow::~TcpMainWindow(){
    ;
}

void TcpMainWindow::initUi()
{
    m_renewal = new RenewalMainWindow(m_client,this);
    m_renewal->show();
}

void TcpMainWindow::renewalBusiness(FileMessage *res)
{
    if(res->head.type == UP_VER_NUM_RES){
        //共8项信息
        qDebug() << res->id;
        qDebug() << res->filename;
        qDebug() << res->c_ver;
        qDebug() << res->p_ver;
        qDebug() << res->c_path;
        qDebug() << res->p_path;
        qDebug() << res->md5;
        qDebug() << res->filesize;

        if(!strcmp(res->c_ver,res->c_ver)){
            qDebug() << "获取更新包数据成功";
            //把每次发过来的数据库文件数据信息存到文件里
            std::ofstream f1("E:\\QtProject\\build-renewal_page-Desktop_Qt_5_9_4_MinGW_32bit-Debug\\information.txt",std::ios::app);
            f1 << res->filename << " ";
            f1 << res->c_path ;
            f1 << "\n";
            f1.close();
        }else{
            QMessageBox::information(m_renewal,"升级失败","错误的升级包");
        }
    }

}

void TcpMainWindow::myRead()
{
    QByteArray buffer = m_client->readAll();
    qDebug() << buffer;
    g_allBuffer.append(buffer);

    int len= (int)(g_allBuffer.size());
    while (len > 0) {
       // 比较一下数据头的长度
       if(len < sizeof(Head)){
           // 和数据头长度对比，小于，说明包头还没收完，继续等下次
           break;
       }
       //通过数据头，取出总长度，类型啥的
       int total_len = ((Head*)g_allBuffer.data())->len;
       // 再次比较封包总长度够不够
       if(len < total_len){ // 全局存的内容长度小于包的总长度，继续等下次
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

    //解开业务type
    int type = *(int*)buffer.data();
    if(type == UP_VER_NUM_RES){
        //升级响应
        qDebug() << "收到升级响应";
        FileMessage *res = (FileMessage*)buffer.data();
        renewalBusiness(res);//调用升级业务响应处理API
    }
    m_heartCheckTimes = HEART_CHECK_TIMES;
}

void TcpMainWindow::heartCheckSlot()
{
    HeartCheckReq req;
    m_client->write((char*)&req,req.head.len); // 发送心跳包
    m_heartCheckTimes--; // 递减阈值
    qDebug() << QString("当前时间:%1 心跳阈值为 %2").arg(QTime::currentTime().toString()).arg(m_heartCheckTimes);
    if(m_heartCheckTimes <= 0){
        // 需要做断线重连操作
        m_client->close();
        m_client->connectToHost("192.168.3.200",8888);
        if(m_client->waitForConnected()){
            m_heartCheckTimes = HEART_CHECK_TIMES; // 重连成功，重置阈值
            qDebug() << "重连成功";
        }
    }
}

void TcpMainWindow::unpackSlot(QByteArray buffer)
{
    QString buf = buffer;
        qDebug() << buf;
        m_heartCheckTimes = HEART_CHECK_TIMES; // 重置阈值
        int type = ((Head*)buffer.data())->type;
        if(type == UP_VER_NUM_RES){
            FileMessage *d = (FileMessage*)buffer.data();
//            qDebug()<<"收到:"<<d->p_ver;
//            buf = d->p_ver;
            //ui->label->setText(buf);
        }else if(type == HEART_CHECK_RES){
            // 收到心跳响应包
            qDebug()<<"收到心跳响应包";
        }
}



