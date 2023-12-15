#ifndef MAPWIDGET_H
#define MAPWIDGET_H

#include <QWidget>
#include <QWebEngineView>
#include <QWebChannel>
#include <QTcpSocket>
#include <QDebug>

#include "channel.h"
#include "delwidget.h"
#include "addwidget.h"
#include "MyProto.h"

namespace Ui {
class MapWidget;
}

class MapWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MapWidget(QWidget *parent = 0);
    explicit MapWidget(QTcpSocket *client,QWidget *parent = 0);
    ~MapWidget();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();
    void onChannelPos(QString f, QString l);
    void clear();
    void search();
    void addSQL(QString);
    void delSQL(int);
public slots:
    void reload();
//    void reload(MapRes *res);S
private:
    Ui::MapWidget *ui;
    QWebEngineView *m_pWebView;

    Channel* m_Channel;
    AddWidget *m_add_window;
    DelWidget *m_del_window;
    QTcpSocket *m_client;
};

#endif // MAPWIDGET_H
