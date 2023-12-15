#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#pragma execution_character_set("utf-8")
#include <QMainWindow>
#include <QPushButton>
#include <QTcpSocket>

#include <QDebug>
#include <QDesktopServices>   //网络
#include <QUrl>               //网络
#include <QTimer>             //定时
#include <QFileDialog>        //文件
#include <QMessageBox>
#include <QPalette>
#include <QBrush>
#include <QTranslator>
#include <QXmlStreamReader>    //解析Xml
#include <QXmlStreamWriter>    //写入Xml

namespace Ui {
class MainWindow;
}

class setMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit setMainWindow(QWidget *parent = 0);
    ~setMainWindow();
protected slots:
    void myhelp();                    //帮助
    void myread();                    //接收数据包
    void on_pushButton_10_clicked();
    void on_pushButton_2_clicked();   //帮助按钮
    void on_pushButton_clicked();
    void on_pushButton_9_clicked();   //发送数据
    void heardCheckSlot();            //断线重连

protected:
    void createSocket();              //设置网络
    void sethelp();                   //设置帮助
    void setTime();                   //设置定时
    void setbackgroud();              //设置背景
    void chineseLanguage();           //中文设置
    void englinshLanguage();          //英文设置
    void traditionalChineseLang();    //繁体中文
    void areb();                      //阿拉伯语
    void readerconfiguration();                 //读取配置
    void writeconfiguration();                  //写入配置
    void mymap(QString str);                       //记录上次加载的背景
    void theme();                                  //主题跟随背景
private slots:
    void on_pushButton_7_clicked();     //个性化
    void on_comboBox_3(int index);      //语言选项

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

    void on_checkBox_clicked();

private:
    Ui::MainWindow *ui;
    //Dialog*head;                       //展示设置界面
    QTcpSocket*m_client;               //套接字
    QTimer*m_time;                     //定时
    int m_heardCheckTime;
    QXmlStreamReader reader;           //xml
    QString filePath;                  //图片路径
    int a;
};

#endif // MAINWINDOW_H
