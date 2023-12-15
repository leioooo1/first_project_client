#ifndef REALTIMESCREENSCREENMAINWINDOW_H
#define REALTIMESCREENSCREENMAINWINDOW_H

#include <QChartView>
#include <QPieSeries>
#include <QPieSlice>
#include <QMainWindow>
#include <QBarSet>
#include <QBarSeries>

#include "showview.h"
#include "historicalquerymainwindow.h"
#include "setmainwindow.h"
#include "logquerymodel.h"
#include "permissionmanagementmodel.h"
#include "warningmainwindow1.h"
#include "mapwidget.h"
// #include "VersionManage.h"

QT_CHARTS_USE_NAMESPACE
using namespace std;
namespace Ui {
    class RealTimescreenScreenMainWindow;
}

class RealTimescreenScreenMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit RealTimescreenScreenMainWindow(QWidget *parent = 0);
    ~RealTimescreenScreenMainWindow();
private:
    void createPieSeries(vector<int> vec);
    void createMaterialBarSeries();
    void createAttendanceBarSeries();
    void initWarnTableWidget();
    void createNavBar();
    bool event(QEvent *event) override;

    void setStackedWidget();
public slots:
    void emitRequestAttendanceSignal();
    void emitRequestMaterialSignal();
    void emitRequestWorkshopDeviceSignal();
    void updatePieSeries();
    void updateMaterialBarSet(DataMaterial * data);
    void updateAttendanceBarSet(DataAttendance *data);
    void updateTableWidget();
    void createViewSlot();

    void onPermissionButtonClicked();
    void onRealtimeScreenButtonClicked();
    void onWarnButtonClicked();
    void onSysSettingButtonClicked();
    void onLogButtonClicked();
    void onHistoryButtonClicked();

    int myTaskC(QByteArray data);
private:
    Ui::RealTimescreenScreenMainWindow *ui;
    QList<QChartView *> m_chartView;
    QVector<QPieSlice *> m_slices;
    QPieSeries *m_pieSeries;
    ShowView *m_tablView;
    QVector<QBarSeries *> m_BarSeries;
    QFrame *m_frame;


    //RenewalMainWindow * m_Renewal;

public:
    ShowView *m_realTimeLine;
};

#endif // REALTIMESCREENSCREENMAINWINDOW_H
