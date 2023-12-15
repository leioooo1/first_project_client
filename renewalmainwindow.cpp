#include "renewalmainwindow.h"
#include "ui_renewalmainwindow.h"
#include <QMouseEvent>
#include <QProcess>
#include <QString>
#include <QStringList>
#include <QFileInfo>
#include <QDebug>
#include <fstream>
#include <string>
#include <string.h>
#include <fstream>
#include <windows.h>
#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <QFile>
#include <QMap>
#pragma execution_character_set("utf-8")

QByteArray g_allBuffer_file;
QMap<QString, QString> fileMap;

RenewalMainWindow::RenewalMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::RenewalMainWindow)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::Dialog|Qt::FramelessWindowHint);//隐藏操作栏
    m_client = nullptr;

    //打开文件读到最新的版本号，以最新的版本号为条件获得要更新的文件
    std::ifstream f2("E:\\QtProject\\build-main-Desktop_Qt_5_9_4_MinGW_32bit-Debug\\1.txt");
    std::string line;
    std::getline(f2,line);
    QString ver_num = QString(QString::fromLocal8Bit(line.c_str()));
    char* ch;
    QByteArray ba = ver_num.toLatin1();
    ch = ba.data();

    UpVerNumReq req;
    strcpy(req.up_ver_num,ch);
    if(m_client){
        qDebug()<<__FUNCTION__<<m_client->write((char*)&req,req.head.len);
    }else{
        qDebug()<<__FUNCTION__<<"socket is null";
    }

    QFile file("E:\\QtProject\\build-renewal_page-Desktop_Qt_5_9_4_MinGW_32bit-Debug\\information.txt");
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) { // 打开文件
        QTextStream in(&file);
        while (!in.atEnd()) { // 逐行读取文件内容
            QString line = in.readLine();
            QStringList fields = line.split(" "); // 分割字符串
            if (fields.size() == 2) {
                QString key = fields[0];
                QString value = fields[1];
                fileMap.insert(key, value); // 将读取的内容存入map中
            }
            ui->textBrowser_2->append("new version");
            for (auto it = fileMap.constBegin(); it != fileMap.constEnd(); ++it) {
                ui->textBrowser_2->append(it.key());
                ui->textBrowser_2->append(it.value());
            }
        }
    }
        file.close(); // 关闭文件

}

RenewalMainWindow::RenewalMainWindow(QTcpSocket *client, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::RenewalMainWindow)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::Dialog|Qt::FramelessWindowHint);//隐藏操作栏
    m_client = client;

    //打开文件读到最新的版本号，以最新的版本号为条件获得要更新的文件
    std::ifstream f2("E:\\QtProject\\build-main-Desktop_Qt_5_9_4_MinGW_32bit-Debug\\1.txt");
    std::string line;
    std::getline(f2,line);
    QString ver_num = QString(QString::fromLocal8Bit(line.c_str()));
    char* ch;
    QByteArray ba = ver_num.toLatin1();
    ch = ba.data();

    UpVerNumReq req;
    strcpy(req.up_ver_num,ch);
    if(m_client){
        qDebug()<<__FUNCTION__<<m_client->write((char*)&req,req.head.len);
    }else{
        qDebug()<<__FUNCTION__<<"socket is null";
    }

}

RenewalMainWindow::~RenewalMainWindow()
{
    delete ui;
}

//鼠标移动窗口事件
void RenewalMainWindow::mousePressEvent(QMouseEvent *event)
{
    m_offset = event->globalPos() - this->pos();//偏移量=鼠标坐标-当前窗口坐标
}
void RenewalMainWindow::mouseMoveEvent(QMouseEvent *event)
{
    move(event->globalPos()-m_offset);//移动(鼠标坐标-偏移量)
}

//启动登录程序
void RenewalMainWindow::login_start(){
    this->close();
    ;//启动软件登录功能
}

//暂不更新按钮(关闭按钮两者功能相同)
void RenewalMainWindow::on_pushButton_2_clicked()
{
    login_start();//调用登录功能
}
//关闭按钮
void RenewalMainWindow::on_pushButton_clicked()
{
    login_start();//调用登录功能
}

//立即更新按钮
void RenewalMainWindow::on_pushButton_3_clicked()
{
    Sleep(5);
    std::ifstream f1("E:\\QtProject\\build-renewal_page-Desktop_Qt_5_9_4_MinGW_32bit-Debug\\information.txt");
    int fileline = 0;
    std::string tmp;
    while(std::getline(f1,tmp,'\n')){
        fileline++;
    }
    qDebug() << fileline;//文件行数

    QFile file("E:\\QtProject\\build-renewal_page-Desktop_Qt_5_9_4_MinGW_32bit-Debug\\information.txt");
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) { // 打开文件
        QTextStream in(&file);
        while (!in.atEnd()) { // 逐行读取文件内容
            QString line = in.readLine();
            QStringList fields = line.split(" "); // 分割字符串
            if (fields.size() == 2) {
                QString key = fields[0];
                QString value = fields[1];
                fileMap.insert(key, value); // 将读取的内容存入map中
            }
        }

        file.close(); // 关闭文件

        // 遍历map数据
        for (auto it = fileMap.constBegin(); it != fileMap.constEnd(); ++it) {
            qDebug() << "Key: " << it.key() << ", Value: " << it.value();
        }
    } else {
        qCritical() << "无法打开文件";
    }

    //map中放了文件名和文件地址
    //我发一个文件名，收一个文件包，解包把文件数据写入到文件里，覆盖的写
        m_client1 = new QTcpSocket(this);
        m_client1->connectToHost("192.168.3.200",8888);
        if(m_client1->waitForConnected()){
            qDebug() << "conn okaaaa";
            connect(m_client1,SIGNAL(readyRead()),this,SLOT(Read()));
            for (auto it = fileMap.constBegin(); it != fileMap.constEnd(); ++it) {
                FileReq fq;
                fq.head.type = FILE_REQ;
                char* ch;
                QByteArray ba = it.key().toLatin1();
                ch = ba.data();
                strcpy(fq.filename,ch);
                fq.head.len = sizeof(fq);
                m_client1->write((char*)&fq,fq.head.len);
            }
        }else{
            qDebug() << "conn failaaaa" << m_client1->errorString();
        }
    this->close();

    //6.Qt启动外部主程序main.exe
    QString helperPath = "E:\\cppcode\\helper\\helper\\Debug\\helper.exe";//辅助启动程序
    QString programPath = "E:\\QtProject\\build-main-Desktop_Qt_5_9_4_MinGW_32bit-Debug\\debug\\main.exe";//主程序

    if(QFileInfo(helperPath).exists() && QFileInfo(programPath).exists())
    {
        qDebug() << "Helper program path exists: " << helperPath;
        qDebug() << "External program path exists: " << programPath;

        QStringList arguments;
        arguments << programPath;

        QProcess *process = new QProcess(this);
        process->setProgram(helperPath);
        process->setArguments(arguments);

        process->start();

        qDebug() << "Helper program started.";

        //等待进程结束
        if(!process->waitForFinished()){
            qWarning() << "Unable to wait for process to finish.";
        }
        qDebug() << "Helper program finished.";

        delete process;

        // 关闭升级程序窗口
        this->close();
    }
    else
    {
        qWarning() << "Helper program path or external program path does not exist.";
    }    
}

void RenewalMainWindow::Read()
{
    QByteArray buffer = m_client1->readAll();
    qDebug() << buffer.size();
    qDebug()<<"buffer:"<<buffer;
    g_allBuffer_file.append(buffer); // 往后追加，存起来

    int len= (int)(g_allBuffer_file.size());
    while (len > 0) {
       // 比较一下数据头的长度
       if(len < sizeof(Head)){
           // 和数据头长度对比，小于，说明包头还没收完，继续等下次
           break;
       }
       //通过数据头，取出总长度，类型啥的
       int total_len = ((Head*)g_allBuffer_file.data())->len;
       // 再次比较封包总长度够不够
       if(len < total_len){ // 全局存的内容长度小于包的总长度，继续等下次
           break;
       }
       // 够了根据类型来进行解包处理
       // 把当前够的这个包，拿去解开并操作
       QByteArray datas = g_allBuffer_file.left(total_len); // 从左边0下标开始截取指定长度内容


       // 截取剩余长度的内容，继续处理剩下的
       //g_allBuffer = g_allBuffer.right(g_allBuffer.size() - total_len); // 从右边末尾计算截取指定长度
       g_allBuffer_file = g_allBuffer_file.mid(total_len); // 从中间指定位置往后截取
       len = g_allBuffer_file.size(); // 刷新len
    }

    //
    int type = *(int*)(buffer.data());
    if(type == FILE_RES){
        FileData* d = (FileData*)buffer.data();
        QString tmp;
        if(fileMap.find(d->filename) != fileMap.end()){
            tmp = fileMap[d->filename];
        }
        tmp += d->filename;
        char* cha;
        QByteArray baa = tmp.toLatin1();
        cha = baa.data();
        qDebug() << cha;
        qDebug() << d->filename;
        qDebug() << d->filecontent;
        std::ofstream f1(cha,std::ios::app | std::ios::binary);
        f1 << d->filecontent;
        f1.close();
    }
}


