#include "warningmainwindow1.h"
#include "ui_warningmainwindow1.h"

#pragma execution_character_set("utf-8")

#include <QDebug>
#include <QVariantMap>
const QStringList g_notifies = {
    QStringLiteral("当前温度低于设定阈值！请及时调整！"),
    QStringLiteral("当前温度高于设定阈值！请及时调整！"),
    QStringLiteral("当前湿度低于设定阈值！请及时调整！"),
    QStringLiteral("当前湿度高于设定阈值！请及时调整！"),
    QStringLiteral("当前光敏度低于设定阈值！请及时调整！"),
    QStringLiteral("当前光敏度高于设定阈值！请及时调整！"),
    QStringLiteral("当前静电低于设定阈值！请及时调整！"),
    QStringLiteral("当前静电高于设定阈值！请及时调整！"),
    QStringLiteral("当前粉尘浓度低于设定阈值！请及时调整！"),
    QStringLiteral("当前粉尘浓度高于设定阈值！请及时调整！")
};
static int counts = 1;
WarningMainWindow1::WarningMainWindow1(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::WarningMainWindow1)
{
    ui->setupUi(this);
    m_u = new WarningMainWindow2(this);
    connect(m_u,SIGNAL(getMaxTem(QString)),this,SLOT(receiveMaxTemData(QString)));
    connect(m_u,SIGNAL(getMinTem(QString)),this,SLOT(receiveMinTemData(QString)));
    connect(m_u,SIGNAL(getMaxHum(QString)),this,SLOT(receiveMaxHumData(QString)));
    connect(m_u,SIGNAL(getMinHum(QString)),this,SLOT(receiveMinHumData(QString)));
    connect(m_u,SIGNAL(getMaxLig(QString)),this,SLOT(receiveMaxLigData(QString)));
    connect(m_u,SIGNAL(getMinLig(QString)),this,SLOT(receiveMinLigData(QString)));
    connect(m_u,SIGNAL(getMaxSta(QString)),this,SLOT(receiveMaxStaData(QString)));
    connect(m_u,SIGNAL(getMinSta(QString)),this,SLOT(receiveMinStaData(QString)));
    connect(m_u,SIGNAL(getMaxDus(QString)),this,SLOT(receiveMaxDusData(QString)));
    connect(m_u,SIGNAL(getMinDus(QString)),this,SLOT(receiveMinDusData(QString)));
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
    m_timer=new QTimer(this);
    connect(m_timer,SIGNAL(timeout()),this,SLOT(Updatetimer()));//timeout信号
    m_model = new QStandardItemModel(5,4,this);
    m_view = new QTableView(this);
    m_view->setModel(m_model);//模型和视图关联
    QStringList labels1=QObject::trUtf8("设备A,设备B,设备C,设备D").simplified().split(",");
    QStringList labels2=QObject::trUtf8("温度,湿度,光敏度,静电,粉尘浓度").simplified().split(",");
    m_model->setHorizontalHeaderLabels(labels1);
    m_model->setVerticalHeaderLabels(labels2);
    m_label1=new QLabel(this);
    m_label2=new QLabel(this);
    m_label3=new QLabel(this);
    m_lin8=new QLineEdit(this);
    m_lin8->setStyleSheet("background-color:white");
    m_label2->setText("红色:高于设定阈值; 蓝色:低于设定阈值; ");
    m_btn4=new QPushButton("警告提示");
    m_btn5=new QPushButton("阈值设置");
    m_btn6=new QPushButton("退出");
    QPushButton* m_btn7 = new QPushButton("定时器设置");

    NotifyManager *manager = new NotifyManager(this);
    manager->setMaxCount(5); // 最大显示消息数，默认5
    manager->setDisplayTime(10000); // 显示时间，默认10000（毫秒）
    manager->setAnimateTime(500); // 动画时间，默认300（毫秒）
    manager->setSpacing(5); // 消息框间距，默认10px
    manager->setCornerMargins(20, 20); // 右下角边距，默认10, 10
    manager->setNotifyWndSize(300, 75); // 消息框大小，默认300, 60
    manager->setDefaultIcon(":/message.png"); // 消息图标，默认":/message.png"
    manager->setShowQueueCount(true); // 是否显示超出最大数未显示的消息数量，默认true
    static int count = 1;
    QObject::connect(m_btn4, &QPushButton::clicked, manager, [&manager]{
         QString text = g_notifies[counts];
        manager->notify(QStringLiteral("警告提示%1").arg(count++), text);
    });
    QObject::connect(manager, &NotifyManager::notifyDetail, this, [&,this](const QVariantMap &data){
        QMessageBox::information(this, data.value("title").toString(), data.value("body").toString());
    });

    connect(m_btn5,SIGNAL(clicked()),this,SLOT(showTestMainwindow()));
    connect(m_btn6,SIGNAL(clicked()),this,SLOT(hideTestMainwindow2()));//退出按钮
    connect(m_btn7,SIGNAL(clicked()),this,SLOT(startMonitoring()));
    QVBoxLayout *layout=new  QVBoxLayout(this);//利用布局将不同的视图放一起显示
    gridlayout = new QGridLayout(this);//网格布局
    gridlayout->addWidget(m_view,0,0,6,10);
    gridlayout->addLayout(layout,0,10,5,11);//1,2是从几行几列开始,3,4是占的行数和列数
    gridlayout->addWidget(m_label1,5,0,1,8);
    gridlayout->addWidget(m_label2,7,0,1,8);
    gridlayout->addWidget(m_label3,6,0,1,1);
    gridlayout->addWidget(m_lin8,0,15,1,1);
    gridlayout->addWidget(m_btn5,2,15,1,1);
    gridlayout->addWidget(m_btn4,3,15,1,1);
    gridlayout->addWidget(m_btn6,4,15,1,1);
    gridlayout->addWidget(m_btn7,1,15,1,1);
    m_view->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_view->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    QWidget *m_widget=new QWidget();
    m_widget->setLayout(gridlayout);
    this->setCentralWidget(m_widget);
    m_timer1=new QTimer(this);
    connect(m_timer1,SIGNAL(timeout()),this,SLOT(currentTime()));
    m_timer1->start(1000);//显示实时时间
    m_player = new QMediaPlayer;
    m_player->setMedia(QUrl::fromLocalFile("D:/QtProject/warningmanagement_qt/jiangbao.wav"));

}

WarningMainWindow1::~WarningMainWindow1()
{
    delete ui;
    delete m_player;
    m_player = nullptr;
}


void WarningMainWindow1::Determine()
{

    m_view->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_view->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    int column = m_model->columnCount();
//温度
    for(int i=0;i<1;++i){
    for(int j=0;j<column;++j){
    if (m_model->index(i,j).data().toInt()<=m_mintem)
    {
        m_model->item(i,j)->setData(QColor("#8ac3ff"),Qt::BackgroundColorRole);
        triggerWarning();
        //QMessageBox::warning(this, "温度警报", "温度低于阈值！");
        counts = 0;
        m_btn4->click();
    }
    if(m_model->index(i,j).data().toInt()>=m_maxtem)
    {
       m_model->item(i,j)->setData(QColor("#fa1748"),Qt::BackgroundColorRole);
       triggerWarning();
      // QMessageBox::warning(this, "温度警报", "温度超过阈值！");
       counts = 1;
       m_btn4->click();
           }
        }
    }
//湿度
     for(int i=1;i<2;++i){
     for(int j=0;j<column;++j){
     if(m_model->index(i,j).data().toInt()<= m_minhum)
     {
        m_model->item(i,j)->setData(QColor("#8ac3ff"),Qt::BackgroundColorRole);
        triggerWarning();
        //QMessageBox::warning(this, "湿度警报", "湿度低于阈值！");
        counts = 2;
        m_btn4->click();
     }
     if(m_model->index(i,j).data().toInt()>=m_maxhum)
     {
        m_model->item(i,j)->setData(QColor("#fa1748"),Qt::BackgroundColorRole);
        triggerWarning();
       // QMessageBox::warning(this, "湿度警报", "湿度超过阈值！");
        counts = 3;
        m_btn4->click();
     }
        }
    }
//光敏度
     for(int i=2;i<3;++i){
     for(int j=0;j<column;++j){
     if (m_model->index(i,j).data().toInt()<=m_minlig)
     {
         m_model->item(i,j)->setData(QColor("#8ac3ff"),Qt::BackgroundColorRole);
         triggerWarning();
        // QMessageBox::warning(this, "光敏度警报", "温度低于阈值！");
         counts = 4;
         m_btn4->click();
     }
     if(m_model->index(i,j).data().toInt()>=m_maxlig)
     {
        m_model->item(i,j)->setData(QColor("#fa1748"),Qt::BackgroundColorRole);
        triggerWarning();
        //QMessageBox::warning(this, "光敏度警报", "光敏度超过阈值！");
        counts = 5;
        m_btn4->click();

            }
         }
     }
//静电
     for(int i=3;i<4;++i){
     for(int j=0;j<column;++j){
     if (m_model->index(i,j).data().toInt()<=m_minsta)
     {
         m_model->item(i,j)->setData(QColor("#8ac3ff"),Qt::BackgroundColorRole);
         triggerWarning();
        // QMessageBox::warning(this, "静电警报", "静电低于阈值！");
         counts = 6;
         m_btn4->click();
     }
     if(m_model->index(i,j).data().toInt()>=m_maxsta)
     {
        m_model->item(i,j)->setData(QColor("#fa1748"),Qt::BackgroundColorRole);
        triggerWarning();
        //QMessageBox::warning(this, "静电警报", "静电超过阈值！");
        counts = 7;
        m_btn4->click();

            }
         }
     }
//粉尘大小
     for(int i=4;i<5;++i){
     for(int j=0;j<column;++j){
     if (m_model->index(i,j).data().toInt()<=m_mindus)
     {
         m_model->item(i,j)->setData(QColor("#8ac3ff"),Qt::BackgroundColorRole);
         triggerWarning();
        // QMessageBox::warning(this, "粉尘浓度警报", "粉尘浓度低于阈值！");
         counts = 8;
         m_btn4->click();
     }
     if(m_model->index(i,j).data().toInt()>=m_maxdus)
     {
        m_model->item(i,j)->setData(QColor("#fa1748"),Qt::BackgroundColorRole);
        triggerWarning();
        //QMessageBox::warning(this, "粉尘浓度警报", "粉尘浓度阈值！");
        counts = 9;
        m_btn4->click();

            }
         }
     }

}

void WarningMainWindow1::currentTime()
{
    QDateTime time=QDateTime::currentDateTime();
    QString str=time.toString("yyyy-MM-dd hh:mm:ss dddd");
    m_label3->setText(str);
}

void WarningMainWindow1::triggerWarning()
{
     m_player->play();
}

void WarningMainWindow1::startMonitoring()
{
   int val = m_lin8->text().toInt();
   m_timer->start(val);
}


void WarningMainWindow1::Updatetimer()
{
    int rand;
    int column = m_model->columnCount();
    for(int i=0;i<1;++i){
        for(int j=0;j<column;++j){//添加第一行温度的数据
         rand=qrand()%(7)+22;
         m_model->setItem(i,j,new QStandardItem(QString::number(rand)));
    }
}
    for(int i=1;i<2;++i){
        for(int j=0;j<column;++j){//添加后第二行湿度的数据
         rand=qrand()%(31)+30;
         m_model->setItem(i,j,new QStandardItem(QString::number(rand)));
    }
  }
    for(int i=2;i<3;++i){
        for(int j=0;j<column;++j){//添加后第三行光敏度的数据
         rand=qrand()%(31)+30;
         m_model->setItem(i,j,new QStandardItem(QString::number(rand)));
    }
  }
    for(int i=3;i<4;++i){
        for(int j=0;j<column;++j){//添加后第四行静电的数据
         rand=qrand()%(31)+30;
         m_model->setItem(i,j,new QStandardItem(QString::number(rand)));
    }
  }
    for(int i=4;i<5;++i){
        for(int j=0;j<column;++j){//添加后第五行气体浓度的数据
         rand=qrand()%(31)+30;
         m_model->setItem(i,j,new QStandardItem(QString::number(rand)));
    }
  }
    Determine();//判断并更改背景颜色
}

void WarningMainWindow1::showTestMainwindow()
{
     m_u->show();
}

void WarningMainWindow1::hideTestMainwindow2()
{

    this->hide();
    delete m_player;
    m_player = nullptr;

}

void WarningMainWindow1::receiveMaxTemData(QString data)
{
     m_maxtem = data.toDouble();
     qDebug() << m_maxtem;
}

void WarningMainWindow1::receiveMinTemData(QString data)
{
     m_mintem= data.toDouble();
     qDebug()<<m_mintem;
}

void WarningMainWindow1::receiveMaxHumData(QString data)
{
      m_maxhum= data.toDouble();
      qDebug() <<m_maxhum;
}

void WarningMainWindow1::receiveMinHumData(QString data)
{
      m_minhum= data.toDouble();
      qDebug() << m_minhum;
}

void WarningMainWindow1::receiveMaxLigData(QString data)
{
      m_maxlig= data.toDouble();
      qDebug() <<m_maxlig;

}

void WarningMainWindow1::receiveMinLigData(QString data)
{
      m_minlig= data.toDouble();
      qDebug() <<m_minlig;
}

void WarningMainWindow1::receiveMaxStaData(QString data)
{
       m_maxsta= data.toDouble();
       qDebug() <<m_maxsta;
}

void WarningMainWindow1::receiveMinStaData(QString data)
{
       m_minsta= data.toDouble();
       qDebug() <<m_minsta;
}

void WarningMainWindow1::receiveMaxDusData(QString data)
{
       m_maxdus= data.toDouble();
       qDebug() <<m_maxdus;
}

void WarningMainWindow1::receiveMinDusData(QString data)
{
       m_mindus= data.toDouble();
       qDebug() << m_mindus;
}
