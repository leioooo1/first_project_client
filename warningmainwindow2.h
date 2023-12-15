#ifndef WARNINGMAINWINDOW2_H
#define WARNINGMAINWINDOW2_H

#include <QMainWindow>
#include <QMessageBox>
#include <QTimer>
#include <QMediaPlayer>
#include <QMediaPlaylist>

namespace Ui {
class WarningMainWindow2;
}

class WarningMainWindow2 : public QMainWindow
{
    Q_OBJECT

public:
    explicit WarningMainWindow2(QWidget *parent = 0);
    ~WarningMainWindow2();
signals:
    void getMaxTem(QString);
    void getMinTem(QString);
    void getMaxHum(QString);
    void getMinHum(QString);
    void getMaxLig(QString);
    void getMinLig(QString);
    void getMaxSta(QString);
    void getMinSta(QString);
    void getMaxDus(QString);
    void getMinDus(QString);
private slots:
     void on_pushButton_clicked();
     void on_pushButton_6_clicked();

private:
    Ui::WarningMainWindow2 *ui;
};

#endif // WARNINGMAINWINDOW2_H
