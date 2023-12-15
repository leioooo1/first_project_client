#include "channel.h"

Channel::Channel(QObject* parent)
    : QObject(parent)
{

}

void Channel::onRecvPostion(const QString f, const QString l)
{
    emit sigCurrentPostion(f, l);
}

void Channel::onLoadMarker()
{
    emit sigMarker();
}

void Channel::markerNumber(int n)
{
    qDebug() << n;
    //emit sigMarkerNumber(n);
}
