#include "myclient.h"
#include "realtimescreenscreenmainwindow.h"
#include "ui_realtimescreenscreenmainwindow.h"

#include <map>
#include <QTime>
#include <QDebug>
#include <QTimer>
#include <QBarCategoryAxis>
#include <QValueAxis>
#include <QDebug>
#include <QPushButton>



HistoricalQueryMainWindow   *m_history ;
setMainWindow               *m_sysSet  ;
logQueryModel               *m_log     ;
PermissionManagementModel   *m_permisson;
WarningMainWindow1          *m_warn;
MapWidget                   *m_mapWidget;
// VersionManage               *m_versionManage;


RealTimescreenScreenMainWindow::RealTimescreenScreenMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::RealTimescreenScreenMainWindow)
{
    ui->setupUi(this);

    this->setWindowState(Qt::WindowMaximized);
    Myclient & client = Myclient::getServer();
    client.Init(8888,"192.168.3.20",this);
    client.Run();
    createNavBar();
    qsrand(QTime::currentTime().msec());
    vector<int> vec;
    vec.push_back(99);
    vec.push_back(89);
    vec.push_back(96);
    vec.push_back(97);
    createPieSeries(vec);
    createMaterialBarSeries();
    createAttendanceBarSeries();
    initWarnTableWidget();
    QTimer *timer = new QTimer(this);
    connect(timer,&QTimer::timeout,this,&RealTimescreenScreenMainWindow::updatePieSeries);
    timer->start(24 * 60 * 60 * 1000);
    QTimer *timer2 = new QTimer(this);
    connect(timer2,&QTimer::timeout,this,&RealTimescreenScreenMainWindow::emitRequestMaterialSignal);
    timer2->start(3000);
    QTimer *timer3 = new QTimer(this);
    connect(timer3,&QTimer::timeout,this,&RealTimescreenScreenMainWindow::emitRequestAttendanceSignal);
    timer3->start(10000);
    QTimer *timer4 = new QTimer(this);
    connect(timer4,&QTimer::timeout,this,&RealTimescreenScreenMainWindow::updateTableWidget);
    timer4->start(4000);

    m_tablView = new ShowView(ui->frame_7);

    m_tablView->setParent(ui->frame_7);
    m_tablView->initTableView();
    connect(m_tablView,&ShowView::createView,this,&RealTimescreenScreenMainWindow::createViewSlot);


    m_realTimeLine = new ShowView(ui->frame);
    m_realTimeLine->setParent(ui->frame);
    QTimer *timer5 = new QTimer(this);
    connect(timer5, &QTimer::timeout, this, &RealTimescreenScreenMainWindow::emitRequestWorkshopDeviceSignal);
    timer5->start(1000);
    m_realTimeLine->createDeviceLineSeries();

    setStackedWidget();
    ui->stackedWidget->setCurrentWidget(ui->realTimeScreen_2);

}

RealTimescreenScreenMainWindow::~RealTimescreenScreenMainWindow()
{
    delete ui;
}

int RealTimescreenScreenMainWindow::myTaskC(QByteArray data)
{
    Head *head = (Head *)(data.data());
    switch (head->type) {
    case TypeInfo::MATERIAl_RES:
    {
        AttendanceRES * attendance =(AttendanceRES *)(data.data());
        updateAttendanceBarSet(attendance->attendance);
    }
        break;
    case TypeInfo::ATTENDANCE_RES:
    {
        AttendanceRES * attendance =(AttendanceRES *)(data.data());
        updateAttendanceBarSet(attendance->attendance);
    }
        break;
    case TypeInfo::WORKSHOPDEVICE_RES:
    {
        WorkshopDeviceRES * workshopDevice =(WorkshopDeviceRES *)(data.data());
        m_realTimeLine->updateDeviceLineSeries(workshopDevice->workshopDevice);
    }
        break;
    default:
        break;
    }
    return 0;
}



bool RealTimescreenScreenMainWindow::event(QEvent *event)
{
    if(event->type() == QEvent::HoverMove){
        if(((QMouseEvent *)event)->x() <= 90){
            m_frame->show();
        }else {
            m_frame->hide();
        }
    }
    return QMainWindow::event(event);
}

void RealTimescreenScreenMainWindow::setStackedWidget()
{
    m_history   = new HistoricalQueryMainWindow(this);
    m_sysSet    = new setMainWindow(this);
    m_log       = new logQueryModel(this);
    m_permisson = new PermissionManagementModel(this);
    m_warn      = new WarningMainWindow1(this);
    m_mapWidget = new MapWidget(ui->frame_3);
    m_mapWidget->setParent(ui->frame_3);
    //m_versionManage = new VersionManage(this);


    ui->stackedWidget->addWidget(m_history);
    ui->stackedWidget->addWidget(m_sysSet);
    ui->stackedWidget->addWidget(m_log);
    ui->stackedWidget->addWidget(m_permisson);
    ui->stackedWidget->addWidget(m_warn);
    //ui->stackedWidget->addWidget(m_versionManage);

    connect(&Myclient::getServer(),&Myclient::realtimeScreen,this,&RealTimescreenScreenMainWindow::myTaskC);
    connect(&Myclient::getServer(),&Myclient::history,m_history,&HistoricalQueryMainWindow::myTaskC);
    connect(&Myclient::getServer(),&Myclient::log,m_log,&logQueryModel::myTaskC);
    connect(&Myclient::getServer(),&Myclient::permisson,m_permisson,&PermissionManagementModel::myTaskC);
 //   connect(&Myclient::getServer(),&Myclient)
}


void RealTimescreenScreenMainWindow::updatePieSeries()
{

    vector<int> vec;
    vec.push_back(80);
    vec.push_back(80);
    vec.push_back(80);
    vec.push_back(30);

    createPieSeries(vec);
}

void RealTimescreenScreenMainWindow::updateMaterialBarSet(DataMaterial * data)
{
    QBarSet * bar1 = this->m_BarSeries.at(0)->barSets().at(0);
    QBarSet * bar2 = this->m_BarSeries.at(0)->barSets().at(1);

    for(int i = 0; i < bar1->count(); ++i){
        int tatol = data[i].total;
        int used = data[i].used;
        bar1->replace(i,tatol);
        bar2->replace(i,used);
    }
    m_chartView[1]->repaint();
}

void RealTimescreenScreenMainWindow::updateAttendanceBarSet(DataAttendance *data)
{

    QBarSet * bar1 = this->m_BarSeries.at(1)->barSets().at(0);
    QBarSet * bar2 = this->m_BarSeries.at(1)->barSets().at(1);
    for(int i = 0; i < bar1->count(); ++i){
        int tatol=data[i].shoudComeCnt;;
        int came= data[i].practicalCnt;

        bar1->replace(i,tatol);
        bar2->replace(i,came);
    }
    m_chartView[2]->repaint();
}

void RealTimescreenScreenMainWindow::updateTableWidget()
{
    QDate data = QDate::currentDate();
    int year = data.year();
    int mouth = data.month();
    int day = data.day();
    QString timeFormated;
    timeFormated = QString::number(year)
                + "/"
                + QString::number(mouth)
                + "/"
                + QString::number(day);

    std::map<int,QVector<QString>> map;
    QVector<QString> temp;
    temp << "一级警告" << "1车间1号设备温度温度过高"
            +timeFormated+" "+QTime::currentTime().toString();
    map.emplace(0,temp);
    temp.clear();
    temp << "二级警告" << "2车间2号设备温度温度过高"
            +timeFormated+" "+QTime::currentTime().toString();
    map.emplace(1,temp);
    temp.clear();
    temp << "三级警告"  << "3车间3号设备温度温度过高"
            +timeFormated+" "+QTime::currentTime().toString();
    map.emplace(2,temp);
    int random = qrand()%3;
    int i = qrand()%8;


    ui->tableWidget->item(i,0)->setText(map[random].at(0));
    ui->tableWidget->item(i,1)->setText(map[random].at(1));
}

void RealTimescreenScreenMainWindow::createViewSlot()
{
//    ShowView *b = new ShowView(ui->frame);
//    b->setParent(ui->frame);
//    b->createDeviceLineSeries();
//    ui->frame->window()->show();
//    qDebug() << ((ShowView *)sender())<<"=====";
//    delete this->m_realTimeLine;

}

void RealTimescreenScreenMainWindow::onRealtimeScreenButtonClicked()
{
    ui->stackedWidget->setCurrentWidget(ui->realTimeScreen_2);
}

void RealTimescreenScreenMainWindow::onPermissionButtonClicked()
{
    ui->stackedWidget->setCurrentWidget(m_permisson);
}

void RealTimescreenScreenMainWindow::onHistoryButtonClicked()
{
    ui->stackedWidget->setCurrentWidget(m_history);
}

void RealTimescreenScreenMainWindow::onWarnButtonClicked()
{
    ui->stackedWidget->setCurrentWidget(m_warn);
}

void RealTimescreenScreenMainWindow::onSysSettingButtonClicked()
{
    ui->stackedWidget->setCurrentWidget(m_sysSet);
}

void RealTimescreenScreenMainWindow::onLogButtonClicked()
{
    ui->stackedWidget->setCurrentWidget(m_log);
}


void RealTimescreenScreenMainWindow::createPieSeries(vector<int> vec)
{
    this->m_slices <<new QPieSlice("产品A "+QString::number(vec[0])+"%",vec[0]);
    this->m_slices <<new QPieSlice("产品B "+QString::number(vec[1])+"%",vec[1]);
    this->m_slices <<new QPieSlice("产品C "+QString::number(vec[2])+"%",vec[2]);
    this->m_slices <<new QPieSlice("产品D "+QString::number(vec[3])+"%",vec[3]);
    this->m_pieSeries = new QPieSeries();
    m_pieSeries->setHoleSize(0.2);
    for (int i = 0; i < m_slices.size(); ++i) {
        m_slices[i]->setLabelVisible(true);
        m_slices[i]->setLabelColor(QColor("white"));
        m_slices[i]->setExploded(true);
        m_slices[i]->setExplodeDistanceFactor(0.1);
        QObject::connect(m_slices[i], &QPieSlice::hovered, m_slices[i], &QPieSlice::setExploded);
        m_pieSeries->append(m_slices[i]);
    }

    QChart *chart = new QChart();
    chart->addSeries(m_pieSeries);

    QPen pen(QColor(7, 255, 239));
    pen.setWidth(1);
    chart->setBackgroundPen(pen); // 设置画笔颜色，画边框的
    chart->setBackgroundBrush(QBrush(QColor(2, 10, 33)));
    QFont titleFont("Arial", 10, QFont::Bold);

    QDate data = QDate::currentDate();
    int year = data.year();
    int mouth = data.month();
    int day = data.day();
    QString timeFormated;
    timeFormated = QString::number(year)
                + " / "
                + QString::number(mouth)
                + " / "
                + QString::number(day);
    chart->setTitle(timeFormated +"产品良品率");
    chart->setTitleFont(titleFont);
    chart->setTitleBrush(QBrush(QColor(7, 255, 239)));
    chart->legend()->setAlignment(Qt::AlignRight);
    chart->setTitleBrush(QBrush(QColor(7, 255, 239)));
    chart->legend()->setLabelColor(QColor("white"));
    chart->setAnimationOptions(QChart::AnimationOption::AllAnimations);
    QChartView * chartView = new QChartView(chart,ui->frame_2);
    chartView->setGeometry(0,0,460,300);
    chartView->setBackgroundBrush(QBrush(QColor(2, 10, 33)));
    chartView->setParent(ui->frame_2);
    chartView->setRenderHint(QPainter::Antialiasing, true);
    chartView->setRenderHint(QPainter::TextAntialiasing, true);
    this->m_chartView << chartView;
}

void RealTimescreenScreenMainWindow::createMaterialBarSeries()
{
    QBarSet * barSet1 = new QBarSet("总量");
    QBarSet * barSet2 = new QBarSet("已使用量");
    *barSet1 << 10 << 30 << 15 << 16 << 16;
    *barSet2 << 1 << 1 << 2 << 3 << 4 ;
    barSet1->setBorderColor(QColor(1,1,1,0));
    barSet1->setBorderColor(QColor(1,1,1,0));
    QBarSeries *barseries = new QBarSeries();
    barseries->setLabelsPosition(QAbstractBarSeries::LabelsOutsideEnd);// QBarSet显示柱子上相应数值
    barseries->setLabelsVisible(true);// QBarSet显示柱子上相应数值
    barseries->append(barSet1);
    barseries->append(barSet2);
    m_BarSeries.push_back(barseries);
    QChart *chart = new QChart();
    chart->addSeries(barseries);
    QPen pen(QColor(7, 255, 239));
    pen.setWidth(1);
    chart->setBackgroundPen(pen); // 设置画笔颜色，画边框的
    chart->setBackgroundBrush(QBrush(QColor(2, 10, 33)));
    QDate data = QDate::currentDate();
    int year = data.year();
    int mouth = data.month();
    int day = data.day();
    QString timeFormated;
    timeFormated = QString::number(year)
                + " / "
                + QString::number(mouth)
                + " / "
                + QString::number(day);
    chart->setTitle(timeFormated +"原料用量");
    QFont titleFont("Arial", 10, QFont::Bold);
    chart->setTitleFont(titleFont);
    chart->setTitleBrush(QBrush(QColor(7, 255, 239)));
    QStringList categories;
    categories << "原料A" << "原料B" << "原料C" << "原料D" << "原料E";
    QBarCategoryAxis *axisX = new QBarCategoryAxis();
    axisX->append(categories);
    chart->setAxisX(axisX, barseries);
    axisX->setLabelsColor("white");
    axisX->setGridLineColor(QColor(1,1,1,0));
    axisX->setRange(QString("原料A"), QString("原料E"));

    QValueAxis *axisY = new QValueAxis();
    chart->setAxisY(axisY, barseries);
    //axisY->setLabelFormat("%.2f");
    axisY->setLinePen(QPen("white"));
    axisY->setLinePenColor(QColor(1,1,1,0));

    axisY->setGridLinePen(QPen(Qt::PenStyle::DashLine));
    axisY->setGridLineColor(QColor(89, 97, 120));
    axisY->setLabelsColor("white");
   // axisY->setRange(0, 35);

    chart->legend()->setVisible(true);
    chart->legend()->setLabelColor(QColor("white"));
    chart->setAnimationOptions(QChart::AnimationOption::GridAxisAnimations);
    //QChart::AnimationOption::GridAxisAnimations是一个动画选项，它表示图表的网格和轴应该进行动画过渡。
    //当你添加新的数据、更改轴范围或更新视图时，这个动画选项会使这些变化以平滑的方式呈现出来。
    QChartView * chartView = new QChartView(chart,ui->frame_5);
    chartView->setGeometry(0,0,717,300);
    chartView->setBackgroundBrush(QBrush(QColor(3, 13, 38)));
    chartView->setParent(ui->frame_5);
    chartView->setRenderHint(QPainter::Antialiasing, true);
    chartView->setRenderHint(QPainter::TextAntialiasing, true);
    this->m_chartView << chartView;
}

void RealTimescreenScreenMainWindow::createAttendanceBarSeries()
{

    QBarSet * barSet1 = new QBarSet("应到人数");
    QBarSet * barSet2 = new QBarSet("实到人数");
    barSet1->setColor(QColor(91, 119, 255));
    barSet2->setColor(QColor(95, 255, 121));
    *barSet1 << 30 << 30 << 30 << 40 ;
    *barSet2 << 29 << 30 << 28 << 23  ;
    barSet1->setBorderColor(QColor(1,1,1,0));
    barSet1->setBorderColor(QColor(1,1,1,0));
    QBarSeries *barseries = new QBarSeries();
    barseries->setLabelsPosition(QAbstractBarSeries::LabelsOutsideEnd);// QBarSet显示柱子上相应数值
    barseries->setLabelsVisible(true);// QBarSet显示柱子上相应数值
    barseries->append(barSet1);
    barseries->append(barSet2);
    m_BarSeries.push_back(barseries);
    QChart *chart = new QChart();
    chart->addSeries(barseries);
    QPen pen(QColor(7, 255, 239));
    pen.setWidth(1);
    chart->setBackgroundPen(pen); // 设置画笔颜色，画边框的
    chart->setBackgroundBrush(QBrush(QColor(3, 13, 38)));
    QDate data = QDate::currentDate();
    int year = data.year();
    int mouth = data.month();
    int day = data.day();
    QString timeFormated;
    timeFormated = QString::number(year)
                + " / "
                + QString::number(mouth)
                + " / "
                + QString::number(day);
    chart->setTitle(timeFormated +"车间人数");
    QFont titleFont("Arial", 10, QFont::Bold);
    chart->setTitleFont(titleFont);
    chart->setTitleBrush(QBrush(QColor(7, 255, 239)));
    QStringList categories;
    categories << "车间A" << "车间B" << "车间C" << "车间D" ;
    QBarCategoryAxis *axisX = new QBarCategoryAxis();
    axisX->append(categories);
    chart->setAxisX(axisX, barseries);
    axisX->setLabelsColor("white");
    axisX->setGridLineColor(QColor(1,1,1,0));
    axisX->setRange(QString("车间A"), QString("车间D"));

    QValueAxis *axisY = new QValueAxis();
    chart->setAxisY(axisY, barseries);
    //axisY->setLabelFormat("%.2f");
    axisY->setLinePen(QPen("white"));
    axisY->setLinePenColor(QColor(1,1,1,0));

    axisY->setGridLinePen(QPen(Qt::PenStyle::DashLine));
    axisY->setGridLineColor(QColor(89, 97, 120));
    axisY->setLabelsColor("white");
    axisY->setRange(0, 50);

    chart->legend()->setVisible(true);
    chart->legend()->setLabelColor(QColor("white"));
    chart->setAnimationOptions(QChart::AnimationOption::GridAxisAnimations);
    //QChart::AnimationOption::GridAxisAnimations是一个动画选项，它表示图表的网格和轴应该进行动画过渡。
    //当你添加新的数据、更改轴范围或更新视图时，这个动画选项会使这些变化以平滑的方式呈现出来。
    QChartView * chartView = new QChartView(chart,ui->frame_6);
    chartView->setParent(ui->frame_6);
    chartView->setGeometry(0,0,717,300);
    chartView->setBackgroundBrush(QBrush(QColor(2, 10, 33)));

    chartView->setRenderHint(QPainter::Antialiasing, true);
    chartView->setRenderHint(QPainter::TextAntialiasing, true);
    this->m_chartView << chartView;
}




void RealTimescreenScreenMainWindow::initWarnTableWidget()
{
    ui->tableWidget->setHorizontalHeaderItem(0,new QTableWidgetItem("警告等级"));
    ui->tableWidget->setHorizontalHeaderItem(1,new QTableWidgetItem("警告详情"));
    ui->tableWidget->horizontalHeader()->setFixedHeight(50);
    ui->tableWidget->horizontalHeader()->setStyleSheet(
                "QHeaderView::section"
                "{background: rgb(3, 13, 38);"
                "color:white;"
                "}");
    ui->tableWidget->horizontalHeader()->show();
    ui->tableWidget->setColumnWidth(0, 142);
    ui->tableWidget->setColumnWidth(1, 280);

    ui->tableWidget->item(0,1)->setTextAlignment(Qt::AlignCenter);
    ui->tableWidget->setFont(QFont("微软雅黑",10));
    QString styleSheet("QTableWidget { gridline-color:rgb(85, 255, 255);"
                       "background-color: rgb(3, 13, 38);"
                       "color:rgb(234,234,234);"
                   "border:1px solid rgb(7,255,239);};"
                        ); //

    ui->tableWidget->setStyleSheet(styleSheet);
    ui->tableWidget-> setEditTriggers(QAbstractItemView::NoEditTriggers);//静止编辑
    for (int i = 0; i < ui->tableWidget->rowCount(); ++i) {
        ui->tableWidget->setRowHeight(i, 48);
    }
    ui->tableWidget->setRowCount(11);
    ui->tableWidget->setColumnCount(2);
    ui->tableWidget->repaint();
    QDate data = QDate::currentDate();
    int year = data.year();
    int mouth = data.month();
    int day = data.day();
    QString timeFormated;
    timeFormated = QString::number(year)
                + "/"
                + QString::number(mouth)
                + "/"
                + QString::number(day);

    for (int i = 0; i < ui->tableWidget->rowCount(); ++i) {
        ui->tableWidget->setItem(i,0,new QTableWidgetItem("一级警告"));
        ui->tableWidget->item(i,0)->setTextColor(QColor("white"));
        ui->tableWidget->item(i,0)->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget->setItem(i,1,new QTableWidgetItem("3车间0号设备温度温度过高"+timeFormated));
        ui->tableWidget->item(i,1)->setTextColor(QColor("white"));
    }
}

void RealTimescreenScreenMainWindow::createNavBar()
{
    m_frame = new QFrame(this);
    m_frame->setGeometry(0,150,80,600);
    m_frame->setStyleSheet("background-color:white");
    QVBoxLayout *layout = new QVBoxLayout(m_frame);
    m_frame->setLayout(layout);

    QPushButton * permissionButton = new QPushButton("权限",this);
    connect(permissionButton, &QPushButton::clicked, this, &RealTimescreenScreenMainWindow::onPermissionButtonClicked);

    QPushButton * realtimeScreenButton = new QPushButton("大屏显示",this);
    connect(realtimeScreenButton, &QPushButton::clicked, this, &RealTimescreenScreenMainWindow::onRealtimeScreenButtonClicked);

    QPushButton * historyButton = new QPushButton("历史查询",this);
    connect(historyButton, &QPushButton::clicked, this, &RealTimescreenScreenMainWindow::onHistoryButtonClicked);

    QPushButton * warnButton = new QPushButton("警告管理",this);
    connect(warnButton, &QPushButton::clicked, this, &RealTimescreenScreenMainWindow::onWarnButtonClicked);

    QPushButton * sysSettingButton = new QPushButton("设置",this);
    connect(sysSettingButton, &QPushButton::clicked, this, &RealTimescreenScreenMainWindow::onSysSettingButtonClicked);

    QPushButton * logButton = new QPushButton("日志",this);
    connect(logButton, &QPushButton::clicked, this, &RealTimescreenScreenMainWindow::onLogButtonClicked);

    layout->addWidget(permissionButton);
    layout->addWidget(realtimeScreenButton);
    layout->addWidget(historyButton);
    layout->addWidget(warnButton);
    layout->addWidget(sysSettingButton);
    layout->addWidget(logButton);
}

void RealTimescreenScreenMainWindow::emitRequestAttendanceSignal()
{
    AttendanceREQ attendance;

    (Myclient::getServer()).myWrite((char *)&attendance,sizeof(AttendanceREQ));
//    qDebug()<<"++++++++++++++++" <<attendance.head.type;
}

void RealTimescreenScreenMainWindow::emitRequestMaterialSignal()
{
    MaterialREQ material;

    (Myclient::getServer()).myWrite((char *)&material,sizeof(material));
 //   qDebug()<< sizeof(MaterialREQ);
}

void RealTimescreenScreenMainWindow::emitRequestWorkshopDeviceSignal()
{
    WorkshopDeviceREQ workshopDevice;
    (Myclient::getServer()).myWrite((char *)&workshopDevice,sizeof(WorkshopDeviceREQ));
 //   qDebug() << sizeof(WorkshopDeviceREQ);
}

