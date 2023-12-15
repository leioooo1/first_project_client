
#include "showview.h"

#include <QPushButton>
#include <QTime>
#include <QTimer>
#include <QValueAxis>
#include <qmath.h>
#include <QDebug>
#include <QDateTimeAxis>
#include <time.h>

ShowView::ShowView(QWidget *parent) : QWidget(parent)
{
   //initTableView();
}

void ShowView::initTableView()
{
    m_model = new QStandardItemModel(7,6,this);
    m_tableView = new QTableView(this);
    m_tableView->setGeometry(0,0,460,305);
    m_tableView->setModel(m_model);
    m_tableView->setAutoScroll(true);
    qsrand(time(0));
    for(int i = 0;i<m_model->rowCount();++i){
        for(int j = 0; j <= m_model->columnCount(); ++j){
            int val = qrand()%91 + 18;
            if(j && j != m_model->columnCount()){
                m_model->setItem(i,j,new QStandardItem(QString::number(val)));
            }
        }
        QPushButton *button = new QPushButton(QString::number(i+1)+"车间");
        button->setStyleSheet("background-color:rgb(2, 188, 185)");
        button->setProperty("id", i); // set custom property
        button->setProperty("name", QString("hello qt tablview %1").arg(i));
        button->setProperty("status", "normal");
        connect(button, SIGNAL(clicked(bool)), this, SLOT(onTableBtnClicked()));
        m_tableView->setIndexWidget(m_model->index(i,0), button);
        m_tableView->setStyleSheet("color:white");
    }

    QStringList labels;
    labels << "车间" <<"温度" <<"湿度" << "PM2.5" << "光感" <<"噪音";

    m_model->setHorizontalHeaderLabels(labels);
}

void ShowView::createDeviceLineSeries()
{
    QLineSeries *lineseriesTemperature  = new QLineSeries();
    QLineSeries *lineseriesHumidity     = new QLineSeries();
    QLineSeries *lineseriesDust         = new QLineSeries();
    QLineSeries *lineseriesLight        = new QLineSeries();
    QLineSeries *lineseriesNoise        = new QLineSeries();
    m_lineSeries.append(lineseriesTemperature);
    m_lineSeries.append(lineseriesHumidity);
    m_lineSeries.append(lineseriesDust);
    m_lineSeries.append(lineseriesLight);
    m_lineSeries.append(lineseriesNoise);


    lineseriesTemperature->setName("温度");
    lineseriesHumidity   ->setName("湿度");
    lineseriesDust       ->setName("灰尘");
    lineseriesLight      ->setName("光感");
    lineseriesNoise      ->setName("噪音");
    QChart *chart            = new QChart();


    chart->addSeries(lineseriesTemperature);
    chart->addSeries(lineseriesHumidity);
    chart->addSeries(lineseriesDust);
    chart->addSeries(lineseriesLight);
    chart->addSeries(lineseriesNoise);


    QDateTimeAxis *axisX = new QDateTimeAxis;
    axisX->setFormat("mm:ss");
    axisX->setLabelsColor("white");
    axisX->setMin(QDateTime::currentDateTime().addMSecs(-1000*5));
    axisX->setMax(QDateTime::currentDateTime());
    axisX->setGridLineColor(QColor(1,1,1,0));
    chart->addAxis(axisX, Qt::AlignBottom);
    lineseriesTemperature->attachAxis(axisX);
    lineseriesHumidity   ->attachAxis(axisX);
    lineseriesDust       ->attachAxis(axisX);
    lineseriesLight      ->attachAxis(axisX);
    lineseriesNoise      ->attachAxis(axisX);

    QtCharts::QValueAxis *axisY = new QtCharts::QValueAxis();
    axisY->setMax(0);
    axisY->setMax(50);
    axisY->setLinePen(QPen("white"));
    axisY->setLinePenColor(QColor(1,1,1,0));
    axisY->setLabelsColor("white");
    axisY->setGridLinePen(QPen(Qt::PenStyle::DashLine));
    axisY->setGridLineColor(QColor(89, 97, 120));
    chart->addAxis(axisY, Qt::AlignLeft);
    lineseriesTemperature->attachAxis(axisY);
    lineseriesHumidity   ->attachAxis(axisY);
    lineseriesDust       ->attachAxis(axisY);
    lineseriesLight      ->attachAxis(axisY);
    lineseriesNoise      ->attachAxis(axisY);

    QPen pen(QColor(7, 255, 239));
    pen.setWidth(1);
    chart->setBackgroundPen(pen); // 设置画笔颜色，画边框的
    chart->setBackgroundBrush(QBrush(QColor(2, 10, 33)));
    QFont titleFont("Arial", 10, QFont::Bold);
    chart->setTitle("实时车间设备信息");
    chart->setTitleFont(titleFont);
    chart->setTitleBrush(QBrush(QColor(7, 255, 239)));
    chart->legend()->setAlignment(Qt::AlignRight);
    chart->setTitleBrush(QBrush(QColor(7, 255, 239)));
    chart->legend()->setLabelColor(QColor("white"));
    chart->setAnimationOptions(QChart::AnimationOption::SeriesAnimations);

    QChartView * chartView          = new QChartView(this);


    chartView           ->setChart(chart);

    m_chartView.append(chartView);

    chartView->setGeometry(0,0,460,305);
    chartView->setBackgroundBrush(QBrush(QColor(2, 10, 33)));
    chartView->setRenderHint(QPainter::Antialiasing, true);
}

void ShowView::onTableBtnClicked()
{
    emit createView();
    qDebug() << "wed";

}

void ShowView::updateDeviceLineSeries(DataWorkshopDevice *workshopDevice)
{
    QDateTime now = QDateTime::currentDateTime();
    qsrand(QTime::currentTime().msecsSinceStartOfDay());

    if(m_lineSeries[0]->count() > 5){
      //  m_lineSeries.at(0)->remove(0);
        m_chartView.at(0)->chart()->axisX()->setMin(QDateTime::currentDateTime().addMSecs(-1000*15));
        m_chartView.at(0)->chart()->axisX()->setMax(QDateTime::currentDateTime());
        m_lineSeries[0]->append(QPointF((qreal)now.toMSecsSinceEpoch(), workshopDevice[0].Temperature));
        m_lineSeries[1]->append(QPointF((qreal)now.toMSecsSinceEpoch(), workshopDevice[0].Humidity));
        m_lineSeries[2]->append(QPointF((qreal)now.toMSecsSinceEpoch(), workshopDevice[0].Dust));
        m_lineSeries[3]->append(QPointF((qreal)now.toMSecsSinceEpoch(), workshopDevice[0].Light));
        m_lineSeries[4]->append(QPointF((qreal)now.toMSecsSinceEpoch(), workshopDevice[0].Noise));
    }else{
        m_lineSeries[0]->append(QPointF((qreal)now.addMSecs(-1000*5).toMSecsSinceEpoch(), workshopDevice[0].Temperature));
        m_lineSeries[1]->append(QPointF((qreal)now.addMSecs(-1000*5).toMSecsSinceEpoch(), workshopDevice[0].Humidity));
        m_lineSeries[2]->append(QPointF((qreal)now.addMSecs(-1000*5).toMSecsSinceEpoch(), workshopDevice[0].Dust));
        m_lineSeries[3]->append(QPointF((qreal)now.addMSecs(-1000*5).toMSecsSinceEpoch(), workshopDevice[0].Light));
        m_lineSeries[4]->append(QPointF((qreal)now.addMSecs(-1000*5).toMSecsSinceEpoch(), workshopDevice[0].Noise));
    }
}
