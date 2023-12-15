#ifndef HISTORICALQUERYMAINWINDOW_H
#define HISTORICALQUERYMAINWINDOW_H

#include <QMainWindow>
#include <QTime>
#include <QDateTime>
#include <iostream>
#include <QStandardItemModel>
#include <QDebug>
#include <QtCharts>
#include <QChartView>
#include <QChart>
#include <QLineSeries>
#include <QValueAxis>
#include <QDateTimeAxis>
#include "MyProto.h"
#include "myclient.h"
using namespace std;
QT_CHARTS_USE_NAMESPACE

namespace Ui {
class HistoricalQueryMainWindow;
}

class HistoricalQueryMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit HistoricalQueryMainWindow(QWidget *parent = 0);
    ~HistoricalQueryMainWindow();

    void displayResult(HistoricalRes *data);

public slots:
    int myTaskC(QByteArray data);
private:
    void initData();


private slots:
    void on_pushButton_clicked();

    void on_changeBtn_clicked();

private:
    Ui::HistoricalQueryMainWindow *ui;
    QStandardItemModel *m_model;
    QChart *m_chart;
    QLineSeries *series1;
    QLineSeries *series2;
    QDateTimeAxis *axisX;
    QValueAxis *axisY;
};

#endif // HISTORICALQUERYMAINWINDOW_H
