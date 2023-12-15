#ifndef SHOWVIEW_H
#define SHOWVIEW_H


#include "MyProto.h"

#include <QWidget>
#include <QTableView>
#include <QStandardItemModel>
#include <QChartView>
#include <QLineSeries>
QT_CHARTS_USE_NAMESPACE

class ShowView : public QWidget
{
    Q_OBJECT
public:
    explicit ShowView(QWidget *parent = nullptr);
    void initTableView();

    void createDeviceLineSeries();
signals:
    void createView();
public slots:
    void onTableBtnClicked();
    void updateDeviceLineSeries(DataWorkshopDevice *workshopDevice);
public:
     QTableView * m_tableView;
     QStandardItemModel * m_model;
     QList<QChartView *> m_chartView;
     QVector<QLineSeries *> m_lineSeries;
};

#endif // SHOWVIEW_H
