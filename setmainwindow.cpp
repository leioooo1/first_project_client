#include "setmainwindow.h"
#include "ui_mainwindow.h"
#define   HEARD_CHECK_TIMES 6;
setMainWindow::setMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_client=new QTcpSocket(this);
//    createSocket();   //设置网络

    connect(ui->pushButton_11,&QPushButton::clicked,this,&setMainWindow::setbackgroud); //背景设置
    connect(ui->comboBox_3,SIGNAL(activated(int)),this,SLOT(on_comboBox_3(int)));    //语言选择
    readerconfiguration();      //读取配置
    //m_time->start(5000);  //定时5秒激发一次
}

setMainWindow::~setMainWindow()
{
    writeconfiguration();    //写入配置
    delete ui;
}

void setMainWindow::myhelp()
{
    //head->close();
    //this->show();
}

void setMainWindow::myread()
{

}

void setMainWindow::on_pushButton_2_clicked()  //帮助
{
    // 在按钮点击事件中添加如下代码
    QString currentPath = QDir::currentPath();
    currentPath+="/HTMLPage1.html";
    QDesktopServices::openUrl(QUrl(currentPath));
}

void setMainWindow::on_pushButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void setMainWindow::createSocket()
{

    m_client->connectToHost("127.0.0.1",8888);   //链接
    if(m_client->waitForConnected()){           //判断链接状态
        qDebug()<<"conn ok";
    }
    else qDebug()<<"conn fail";
}

void setMainWindow::sethelp()
{
    //head=new Dialog(this);
    //head->show();
}

//void MainWindow::setTime()
//{
  //m_time=new QTimer(this);
  //m_heardCheckTime=HEARD_CHECK_TIMES;
//}

void setMainWindow::setbackgroud()           //设置背景
{

            filePath = QFileDialog::getOpenFileName(this, "选择图片", "", "Images (*.png *.jpg *.jpeg)");
            qDebug()<<"图片路径"<<filePath;
            if (!filePath.isEmpty()) {
                // 创建一个QPixmap对象，加载用户选择的图片文件
                QPixmap image(filePath);
                if (image.isNull()) {
                    QMessageBox::critical(this, "fail", "not load");
                    return;
                }

                // 创建一个QPalette对象，设置背景色为用户选择的图片
                QPalette palette;
                palette.setBrush(QPalette::Background, QBrush(image));

                // 将窗口的背景色设置为用户选择的图片
                setAutoFillBackground(true);
                setPalette(palette);
                }
                theme();                          //主题跟随背景
}

void setMainWindow::chineseLanguage()  //中文设置
{

    QTranslator translator;
    translator.load("zh_tr.qm");
    qApp->installTranslator(&translator);
    this->ui->retranslateUi(this);
    //qDebug()<<"切换为中文";
}
void setMainWindow::englinshLanguage()  //英文设置
{
    QTranslator translator;
    translator.load("en_tr.qm");
    qApp->installTranslator(&translator);
    this->ui->retranslateUi(this);
    //qDebug()<<"切换为英文";
}

void setMainWindow::traditionalChineseLang() //繁体中文
{
    QTranslator translator;
    translator.load("tr_tr.qm");
    qApp->installTranslator(&translator);
    this->ui->retranslateUi(this);
  //  qDebug()<<"切换为繁体";
}

void setMainWindow::areb()  //阿拉伯语
{
    QTranslator translator;
    translator.load("ar_tr.qm");
    qApp->installTranslator(&translator);
    this->ui->retranslateUi(this);
  //  qDebug()<<"切换为阿拉伯文";
}

void setMainWindow::readerconfiguration()          //读取配置
{
  QString str=QDir::currentPath();             //获得工作路径
  str+="/~vsAFBC.xml";
  QFile f(str);
  if(!f.open(QFile::ReadOnly|QFile::Text)){
   //   qDebug()<<"打开失败";
      return;
  }
  QXmlStreamReader reader(&f);
  while(!reader.atEnd()){
      reader.readNext();
  //    qDebug()<<"打开成功";
      if(reader.isStartElement()&&reader.name()=="background"){
          mymap(reader.readElementText());
          continue;
      }

           if(reader.isStartElement()&&reader.name()=="language"){
              // qDebug()<<"读取语言";
               QString str=reader.readElementText();
               qDebug()<<str;
               a=str.toInt();      //给a重新赋索引值
               qDebug()<<a;
              if(str=="0"){
                  chineseLanguage();
                 // qDebug()<<"中文";
                  continue;
              }
              if(str=="1"){
                //  qDebug()<<"英文";
                  englinshLanguage();
                  continue;
              }
              if(str=="2"){
                  traditionalChineseLang();
                 // qDebug()<<"繁体";
                  continue;
              }
              if(str=="3"){
                  areb();
                 // qDebug()<<"阿拉伯";
                  continue;
              }
          }
           if(reader.isStartElement()&&reader.name()=="renew"){
             //  qDebug()<<"版本更新";
               if(reader.readElementText()=="1"){
               ui->checkBox->setChecked(true);
               }
               else ui->checkBox->setChecked(false);
           }
  }
  f.close();
}

void setMainWindow::writeconfiguration()      //写入配置
{
   QString str=QDir::currentPath();
   str+="/~vsAFBC.xml";
   QFile f(str);
   if(!f.open(QIODevice::WriteOnly|QIODevice::Text)){
       qDebug()<<"无法打开";
       return;
   }
   QXmlStreamWriter write(&f);
   write.setAutoFormatting(true);    //格式化提高可读性
   write.writeStartDocument();       //写文件头<?xml version="1.0"  encoding "UTF-8"?>
   write.writeStartElement("configuration");  //根节点
   //QString backgroud(filePath.data());
   write.writeStartElement("background");
   write.writeCDATA(filePath);
   write.writeEndElement();
   write.writeTextElement("language", QString::number(a));
   write.writeTextElement("renew",QString::number(ui->checkBox->isChecked()));
   write.writeEndDocument();
   f.close();
}

void setMainWindow::mymap(QString str)             //加载上次的背景
{
      //  qDebug()<<"加载上次的背景";
        filePath=str;
        if(!filePath.isEmpty()){
        QPixmap image(filePath);
        if (image.isNull()) {
      //      QMessageBox::critical(this, "错误", "无法加载选定的图片");
            return;
        }

        // 创建一个QPalette对象，设置背景色为用户选择的图片
        QPalette palette;
        palette.setBrush(QPalette::Background, QBrush(image));

        // 将窗口的背景色设置为用户选择的图片
        setAutoFillBackground(true);
        setPalette(palette);
        }
        theme();                                      //主题跟随背景
}

void setMainWindow::theme()                              //主题跟随背景
{
  QString str="QPushButton { background-image: url(";
  str+=filePath;
  str+="); }";
  qApp->setStyleSheet(str);
 //QString t="QComboBox { background-image: url(";
 //str+=filePath;
 //str+="); }";
 //qApp->setStyleSheet(t);
 //QString r="QPlainTextEdit { background-image: url(";
 //str+=filePath;
 //str+="); }";
 //qApp->setStyleSheet(r);
}

void setMainWindow::on_pushButton_10_clicked()
{

}

void setMainWindow::on_pushButton_9_clicked()  //发包
{
    //int size=strlen(ui->plainTextEdit->toPlainText().toLocal8Bit().data())+1;
    //m_client->write(ui->plainTextEdit->toPlainText().toLocal8Bit().data(),size);
    //m_heardCheckTime=HEARD_CHECK_TIMES;
}

void setMainWindow::heardCheckSlot()
{
   qDebug()<<m_heardCheckTime--;
   if(m_heardCheckTime<=0){
       m_client->close();
       m_heardCheckTime=HEARD_CHECK_TIMES;
       m_client->connectToHost("127.0.0.1",8888);
       if(m_client->waitForConnected()){
           qDebug()<<"重连成功";
       }
       else {qDebug()<<"重连失败";}
   }
}

void setMainWindow::on_pushButton_7_clicked()   //个性化
{
    ui->stackedWidget->setCurrentIndex(1);
}


void setMainWindow::on_comboBox_3(int index) //选择语言
{
    a=index;        //存储用户所选择的索引
    switch (index) {
    case 0:
        chineseLanguage();
        break;
    case 1:
        englinshLanguage();
        break;
    case 2:
        traditionalChineseLang();
        break;
    case 3:
        areb();
        break;
    }
}

void setMainWindow::on_pushButton_4_clicked()  //切换账号
{

}

void setMainWindow::on_pushButton_5_clicked()  //服务
{
   ui->stackedWidget->setCurrentIndex(2);
}

void setMainWindow::on_checkBox_clicked()     //自动更新
{

}
