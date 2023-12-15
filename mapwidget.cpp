#include "mapwidget.h"
#include "ui_mapwidget.h"

MapWidget::MapWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MapWidget),
    m_pWebView(new QWebEngineView(this))
{
    ui->setupUi(this);
    m_Channel = new Channel(this);
    connect(m_Channel, &Channel::sigCurrentPostion, this, &MapWidget::onChannelPos);
    connect(m_Channel, SIGNAL(sigMarker()), this, SLOT(reload()));
    QWebChannel* pWebChannel = new QWebChannel(this);
    pWebChannel->registerObject("window", (QObject*)m_Channel);
    this->m_pWebView->page()->setWebChannel(pWebChannel);
    this->m_pWebView->page()->load(QUrl("F:///QtprojectMayi/real_time_screen/baidumap.html"));
    this->m_pWebView->setGeometry(0,0,800,560);
    this->m_pWebView->show();
    this->setGeometry(0,0,990,610);
    //search();
}

MapWidget::~MapWidget()
{
    delete ui;
}

void MapWidget::on_pushButton_clicked()
{
    if(m_add_window == nullptr){
        m_add_window = new AddWidget(this);
        m_add_window->show();
        connect(m_add_window,SIGNAL(mysignal(QString)),this,SLOT(addSQL(QString)));
    }
    m_add_window->show();
}

void MapWidget::on_pushButton_2_clicked()
{
    if(m_del_window == nullptr){
        m_del_window = new DelWidget(this);
        m_del_window->show();
        connect(m_del_window,SIGNAL(mysignal(int)),this,SLOT(delSQL(int)));
    }
    m_del_window->show();
}

void MapWidget::clear()
{
    this->m_pWebView->page()->runJavaScript(QString("removeMarker()"));
}

void MapWidget::search()
{
    MapSearch req;
    m_client->write((char*)&req,req.head.len);
}

void MapWidget::onChannelPos(QString f, QString l)
{
    this->ui->label->setText(QString("%1,%2").arg(f).arg(l));
}

void MapWidget::addSQL(QString str)
{
    MapAdd add;
    strcpy(add.data,str.toLocal8Bit());
    //m_client->write((char*)&add,add.head.len);
    qDebug()<<str;
    m_add_window->hide();
    reload();
    //clear();
    //search();
}

void MapWidget::delSQL(int n)
{
    MapDelete del;
    del.markerNumber = n;
    //m_client->write((char*)&del,del.head.len);
    qDebug() << n;
    m_del_window->hide();
    reload();
    clear();
    //search();
}


void MapWidget::reload()//加载点位。
{
    this->m_pWebView->page()->runJavaScript(QString("removeMarker()"));
    QVector<QString> vec;
    vec.push_back("101.2253,32.1256");
    vec.push_back("108.9536,34.2574");
    vec.push_back("111.2253,36.9230");
    vec.push_back("119.6553,33.1786");
    vec.push_back("132.2253,46.6536");
    vec.push_back("111.2253,41.6536");
    vec.push_back("125.2253,29.6536");
    vec.push_back("99.2253,32.6536");
    vec.push_back("100.2253,45.6536");
    for(int i = 0;i < vec.size(); ++i){
        QStringList list = vec[i].split(",");
        QString lng = list[0];
        QString lat = list[1];
        this->m_pWebView->page()->runJavaScript(QString("SetPoint(%1,%2,%3)").arg(lng).arg(lat).arg(i + 1));
    }
}

//void MapWidget::reload(MapRes *res)
//{
//    QString str = QString::fromUtf8(res->data);
//    int n = res->number;
//    QStringList list = str.split(",");
//    QString lng = list[0];
//    QString lat = list[1];
//    this->m_pWebView->page()->runJavaScript(QString("SetPoint(%1,%2,%3)").arg(lng).arg(lat).arg(n));
//}
