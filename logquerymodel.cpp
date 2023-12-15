#include "logquerymodel.h"
#include "ui_logquerymodel.h"

logQueryModel::logQueryModel(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::logQueryModel)
{
    ui->setupUi(this);
    m_model = new QStandardItemModel(20,6);
//    m_model->setHorizontalHeaderItem(0, new QStandardItem("时间"));
//    m_model->setHorizontalHeaderItem(1, new QStandardItem("进程号"));
//    m_model->setHorizontalHeaderItem(2, new QStandardItem("线程号"));
//    m_model->setHorizontalHeaderItem(3, new QStandardItem("文件名"));
//    m_model->setHorizontalHeaderItem(4, new QStandardItem("函数名"));
//    m_model->setHorizontalHeaderItem(5, new QStandardItem("日志信息"));

    m_model->setHorizontalHeaderItem(0, new QStandardItem("Time"));
    m_model->setHorizontalHeaderItem(1, new QStandardItem("proc"));
    m_model->setHorizontalHeaderItem(2, new QStandardItem("thread"));
    m_model->setHorizontalHeaderItem(3, new QStandardItem("file"));
    m_model->setHorizontalHeaderItem(4, new QStandardItem("function"));
    m_model->setHorizontalHeaderItem(5, new QStandardItem("logINfo"));

    ui->tableView->setModel(m_model);

    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}

logQueryModel::~logQueryModel()
{
    delete ui;
}



int logQueryModel::myTaskC(QByteArray data)
{
    Head * head = (Head*)data.data();
    if(head->type == SEARCHLOG_RES){
        searchLogRes * res = (searchLogRes*)data.data();
        for(int row = 0; row < res->cnt; ++row){
            m_model->setItem(row,0,new QStandardItem(res->info[row].time));
            m_model->setItem(row,1,new QStandardItem((QString::number(res->info[row].processId))));
            m_model->setItem(row,2,new QStandardItem(QString::number(res->info[row].threadId)));
            m_model->setItem(row,3,new QStandardItem(res->info[row].fileName));
            m_model->setItem(row,4,new QStandardItem(res->info[row].functionName));
            m_model->setItem(row,5,new QStandardItem(res->info[row].logInfomation));
        }
        ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
         ui->tableView->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
        return 0;
    }
    return -1;
}


void logQueryModel::on_search_log_clicked()
{
    QString beginTime = ui->dateEdit->text();
    QString endTime = ui->dateEdit_2->text();
    QString logType = ui->comboBox->currentText();
    qDebug()<<beginTime;
    qDebug()<<endTime;
    qDebug()<<logType;


    strcpy(m_searchLogReq.endTime,endTime.toLocal8Bit().data());
    strcpy(m_searchLogReq.beginTime,beginTime.toLocal8Bit().data());
    strcpy(m_searchLogReq.logType,logType.toUtf8().data());
   // strncpy(m_searchLogReq.beginTime, beginTime.toUtf8().data(), sizeof(m_searchLogReq.beginTime) - 1);
   // m_searchLogReq.beginTime[sizeof(m_searchLogReq.beginTime) - 1] = '\0';
   // strncpy(m_searchLogReq.endTime, endTime.toUtf8().data(), sizeof(m_searchLogReq.endTime) - 1); m_searchLogReq.endTime[sizeof(m_searchLogReq.endTime) - 1] = '\0';
   // strncpy(m_searchLogReq.logType, logType.toUtf8().data(), sizeof(m_searchLogReq.logType) - 1); m_searchLogReq.logType[sizeof(m_searchLogReq.logType) - 1] = '\0';
    qDebug()<<"**********";
    qDebug()<<m_searchLogReq.beginTime;
    qDebug()<<m_searchLogReq.endTime;
    qDebug()<<m_searchLogReq.logType;

    Myclient& myclient = Myclient::getServer();
    myclient.myWrite((char*)&m_searchLogReq, m_searchLogReq.head.len);
}
