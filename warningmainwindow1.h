#ifndef WARNINGMAINWINDOW1_H
#define WARNINGMAINWINDOW1_H
#pragma execution_character_set("utf-8")
#include <QMainWindow>
#include <QTimeEdit>
#include <QHBoxLayout>
#include <QTimer>
#include <QTableView>
#include <QLayoutItem>
#include <QTimeEdit>
#include <QTime>
#include <QStandardItemModel>
#include <QPushButton>
#include <QLabel>
#include <QGridLayout>
#include <QTableWidgetItem>
#include <QSound>
#include <QMediaPlayer>
#include <QApplication>
#include <QTcpSocket>
#include <QMediaPlaylist>
#include <QLineEdit>

#include "warningmainwindow2.h"
#include "notifymanager.h"

namespace Ui {
class WarningMainWindow1;
}

class WarningMainWindow1 : public QMainWindow
{
    Q_OBJECT

public:
    explicit WarningMainWindow1(QWidget *parent = 0);
    ~WarningMainWindow1();
    void Determine();//判断是否超差或者是极限值并改背景色
    void triggerWarning();
signals:
    void unpackSignal(int datas);
public slots:
    void Updatetimer();//定时器槽函数
    void currentTime();//显示实时时
    void showTestMainwindow();
    void hideTestMainwindow2();
    void startMonitoring();
private slots:
    void receiveMaxTemData(QString data);
    void receiveMinTemData(QString data);
    void receiveMaxHumData(QString data);
    void receiveMinHumData(QString data);
    void receiveMaxLigData(QString data);
    void receiveMinLigData(QString data);
    void receiveMaxStaData(QString data);
    void receiveMinStaData(QString data);
    void receiveMaxDusData(QString data);
    void receiveMinDusData(QString data);
private:
    Ui::WarningMainWindow1 *ui;
    QTimer*m_timer;
    QTimer*m_timer1;
    QStandardItemModel *m_model;
    QTableView*m_view;
    QLabel*m_label1;
    QLabel*m_label2;
    QLabel*m_label3;
    QPushButton*m_btn4;
    QPushButton*m_btn5;
    QPushButton*m_btn6;
    QLineEdit *m_lin8;
    QGridLayout*gridlayout;
    WarningMainWindow2 *m_u;
    QMediaPlayer * m_player;
    NotifyManager *m_manager;
    QString text1;



    double m_maxtem = 100;
    double m_mintem = 100;
    double m_maxhum = 100;
    double m_minhum = 100;
    double m_maxlig = 100;
    double m_minlig = 100;
    double m_maxsta = 100;
    double m_minsta = 100;
    double m_maxdus = 100;
    double m_mindus = 100;


};

#endif // WARNINGMAINWINDOW1_H
