#ifndef CHANNEL_H
#define CHANNEL_H

#include <QObject>
#include <QString>
#include <QDebug>
// #include <QWebChannel>

class Channel : public QObject
{
    Q_OBJECT
public:
    explicit Channel(QObject* parent = nullptr);

signals:
    void sigCurrentPostion(const QString, const QString);

    void sigMarkerNumber(int);
    void sigMarker();
public slots:
    void onRecvPostion(const QString, const QString);
    void onLoadMarker();
    void markerNumber(int n);
};

#endif // CHANNEL_H
