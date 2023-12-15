#ifndef RENEWALMAINWINDOW_H
#define RENEWALMAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>

#include "MyProto.h"

namespace Ui {
class RenewalMainWindow;
}

class RenewalMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit RenewalMainWindow(QWidget *parent = 0);
    explicit RenewalMainWindow(QTcpSocket * client,QWidget *parent = 0);
    ~RenewalMainWindow();

    void login_start();

protected slots:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

public slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_3_clicked();
    void Read();

private:
    Ui::RenewalMainWindow *ui;
    QPoint m_offset;
    QTcpSocket *m_client;
    QTcpSocket *m_client1;
};

#endif // RENEWALMAINWINDOW_H
