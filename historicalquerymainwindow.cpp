#include "historicalquerymainwindow.h"
#include "ui_historicalquerymainwindow.h"

#include "MyProto.h"

HistoricalQueryMainWindow::HistoricalQueryMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::HistoricalQueryMainWindow)
{
    ui->setupUi(this);
    Myclient::getServer().Init(8888,"192.168.3.18",this);
    initData();
}

int HistoricalQueryMainWindow::myTaskC(QByteArray data)
{
    Head *head = (Head *)(data.data());
    if(head->type == HISTORICAL_QUERY_RES)
    {
        HistoricalRes *history = (HistoricalRes *)(data.data());
        displayResult(history);
    }
    return 0;
}


HistoricalQueryMainWindow::~HistoricalQueryMainWindow()
{
    delete ui;
}

void HistoricalQueryMainWindow::initData()
{
    QStringList labels;
    labels << "日期"
           << "生产数量"
           << "良品数量";
    m_model = new QStandardItemModel(this);
    ui->tableView->setModel(m_model);
    m_model->setColumnCount(labels.size());
    m_model->setHorizontalHeaderLabels(labels);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    //折线图
    m_chart = new QChart();
    ui->chartView->setChart(m_chart);

    series1 = new QLineSeries();
    series2 = new QLineSeries();
    series1->setName("生产总量");
    series2->setName("良品数量");

    // 创建坐标轴
    axisX = new QDateTimeAxis;    // X轴
    axisX->setFormat("yyyy--MM--dd hh:mm::ss");
    axisX->setTitleText("日期");

    axisY = new QValueAxis;    // Y轴
    axisY->setRange(0, 100);               // Y轴范围
    axisY->setTitleText("数量");
    axisY->setMinorTickCount(4);           // s设置Y轴刻度
}

void HistoricalQueryMainWindow::displayResult(HistoricalRes *data)
{
    series1->clear();
    series2->clear();

    int maxVal = 0;
    int minVal = 99999;
    int sum = 0;
    int yieldSum = 0;
    for (int i = 0; i < data->cnt; ++i)
    {
        for (int j = 0; j < 3; ++j)
        {
            if(j == 0)
            {
                QString datetime = QString::fromLocal8Bit(data->data[i].p_date);
                m_model->setItem(i,j,new QStandardItem(datetime));
            }
            else if(j == 1)
            {
                int val = data->data[i].p_count;
                m_model->setItem(i,j,new QStandardItem(QString::number(val)));
                maxVal = maxVal > val ? maxVal : val;
                minVal = minVal < val ? minVal : val;
                sum += val;

                series1->append(QPointF(QDateTime::fromString(data->data[i].p_date,"yyyy--MM--dd hh::mm::ss").toMSecsSinceEpoch(),data->data[i].p_count));
            }
            else if(j == 2)
            {
                int val = data->data[i].p_yield;
                m_model->setItem(i,j,new QStandardItem(QString::number(val)));
                yieldSum += val;

                series2->append(QPointF(QDateTime::fromString(data->data[i].p_date,"yyyy--MM--dd hh::mm::ss").toMSecsSinceEpoch(),data->data[i].p_yield));
            }
        }
    }

    m_chart->addSeries(series1);
    m_chart->addSeries(series2);

    // 设置X于Y轴数据集
    m_chart->setAxisX(axisX, series1);   // 为序列设置坐标轴
    m_chart->setAxisY(axisY, series1);

    m_chart->setAxisX(axisX, series2);   // 为序列设置坐标轴
    m_chart->setAxisY(axisY, series2);

    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->max_label->setText(QString::number(maxVal));
    ui->min_label->setText(QString::number(minVal));
    ui->avg_label->setText(QString::number((float)sum / 10,'f',2));
    double anomalyVal = (double)(sum - yieldSum) / sum * 100;
    ui->anomaly_label->setText(QString::number(anomalyVal,'f',0) + '%');
    double yieldVal = (double)yieldSum / sum * 100;
    ui->yield_label->setText(QString::number(yieldVal,'f',0) + '%');


}


void HistoricalQueryMainWindow::on_pushButton_clicked()
{
    HistoricalReq req;
    strcpy(req.startDate,ui->begin_dateTimeEdit->text().toStdString().c_str());
    strcpy(req.endDate,ui->end_dateTimeEdit->text().toStdString().c_str());
    (Myclient::getServer()).myWrite((char *)(&req),req.head.len);
}


void HistoricalQueryMainWindow::on_changeBtn_clicked()
{
    if(ui->stackedWidget->currentIndex() == 0)
    {
        ui->stackedWidget->setCurrentWidget(ui->page_2);
    }
    else if(ui->stackedWidget->currentIndex() == 1)
    {
        ui->stackedWidget->setCurrentWidget(ui->page);
    }
}
