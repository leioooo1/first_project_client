#include "Loginmainwindow.h"
#include "ui_LoginMainwindow.h"
#include <Qpainter>
#include <QMouseEvent>
#include "signup.h"
#include "myclient.h"
#include "realtimescreenscreenmainwindow.h"
#include <QGraphicsDropShadowEffect>

Loginmainwindow::Loginmainwindow(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::Loginmainwindow)
{

    ui->setupUi(this);


    loginInit();//登录界面初始化

    connect(ui->radioButton_password,&QRadioButton::clicked,this,&Loginmainwindow::on_radioButton_password_clicked);
    //去窗口边框
    setWindowFlags(Qt::FramelessWindowHint | windowFlags());

       //把窗口背景设置为透明;
           setAttribute(Qt::WA_TranslucentBackground);
    QPixmap *pix = new QPixmap(":/src/mediaSrc/蚂蚁logo.png");
    QSize sz = ui->label_image->size();
    ui->label_image->setPixmap(pix->scaled(sz));

    //设置阴影
    QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect(this);
    shadow->setOffset(-3, 0);
    shadow->setColor(QColor("#888888"));
    shadow->setBlurRadius(30);
    ui->label_image->setGraphicsEffect(shadow);
  //  //初始化数据库
//    sqlite_Init();

}

Loginmainwindow::~Loginmainwindow()
{

    delete ui;
}

//void MainWindow::setMarginWidth(const int &iWidth)
//{
//    m_iMarginWidth = iWidth;
//}

/*
 * void MainWindow::serResizable(bool bCanResize)
{
    m_bCanResize = bCanResize;
}*/

void Loginmainwindow::mouseMoveEvent(QMouseEvent *e)
{
    if(e->buttons() & Qt::LeftButton)
       {
           //移到左上角
           move(e->globalPos() - p);
       }
}

void Loginmainwindow::mousePressEvent(QMouseEvent *e)
{
    if(e->button() == Qt::LeftButton)
        {
            //求坐标差值
            //当前点击坐标-窗口左上角坐标
            p = e->globalPos() - this->frameGeometry().topLeft();
    }
}

/*bool MainWindow::nativeEvent(const QByteArray &eventType, void *message, long *result)
{
    if (!m_bCanResize)
        {
            return QWidget::nativeEvent(eventType,message,result);
        }

        MSG* msg = (MSG*)message;
        switch(msg->message)
            {
            case WM_NCHITTEST:

                int xPos = GET_X_LPARAM(msg->lParam) - this->frameGeometry().x();
                int yPos = GET_Y_LPARAM(msg->lParam) - this->frameGeometry().y();
                if(xPos < m_iMarginWidth && yPos<m_iMarginWidth)                    //左上角
                    *result = HTTOPLEFT;
                else if(xPos>=width()-m_iMarginWidth&&yPos<m_iMarginWidth)          //右上角
                    *result = HTTOPRIGHT;
                else if(xPos<m_iMarginWidth&&yPos>=height()-m_iMarginWidth)         //左下角
                    *result = HTBOTTOMLEFT;
                else if(xPos>=width()-m_iMarginWidth&&yPos>=height()-m_iMarginWidth)//右下角
                    *result = HTBOTTOMRIGHT;
                else if(xPos < m_iMarginWidth)                                     //左边
                    *result =  HTLEFT;
                else if(xPos>=width()-m_iMarginWidth)                              //右边
                    *result = HTRIGHT;
                else if(yPos<m_iMarginWidth)                                       //上边
                    *result = HTTOP;
                else if(yPos>=height()-m_iMarginWidth)                             //下边
                    *result = HTBOTTOM;
                else              //其他部分不做处理，返回false，留给其他事件处理器处理
                   return false;
                return true;
            }
            return false;         //此处返回false，留给其他事件处理器处理
}*/

void sqlite_Init()
{


}
void Loginmainwindow::on_btn_signin_clicked()
{

    //sqlite_Init();

    QString username = ui->lineEdit_username->text();
    QString password = ui->lineEdit_password->text();
    QString sql=QString("select * from user where username='%1' and password='%2'")
            .arg(username).arg(password);
   /* rememberMe = ui->radioButton_password->isChecked();
    if(rememberMe){
        QFile file("login.txt");
        if(file.open(QIODevice::WriteOnly|QIODevice::Text)){
            QTextStream out(&file);
            out<<username<<endl;
            out<<password<<endl;
            file.close();
        }
    }*/
    LoginReq loginReq;
    strcpy_s(loginReq.username,16,username.toUtf8().data());
    strcpy_s(loginReq.password,16,password.toUtf8().data());

    char*toLoginReq = (char*)&loginReq;
    QString toutf8(toLoginReq);
    Myclient::getServer().myWrite(toutf8.toUtf8().data(),loginReq.head.len);//以utf8发用户名和密码

    //创建执行语句对象
    //QSqlQuery query(sql);
    //判断执行结果
    RealTimescreenScreenMainWindow * realtime = new RealTimescreenScreenMainWindow(this);
    realtime->show();
    this->hide();

}

void Loginmainwindow::on_btn_signup_clicked()
{
    this->close();
    Signup *s = new Signup;
    s->show();

}

void Loginmainwindow::on_toolButton_min_clicked()
{
    Loginmainwindow();
}

void Loginmainwindow::on_toolButton_close_clicked()
{
    // 如果用户没有选择记住密码，则删除本地文件中的用户名和密码
   /* if (!rememberMe) {
        QFile::remove("login.txt");
    }*/
    close();

}

void Loginmainwindow::on_toolButton_max_clicked()
{
    showMaximized();
}

void Loginmainwindow::on_radioButton_password_clicked()
{

}

void Loginmainwindow::LoginWindow(QByteArray data)
{
    Head *head = (Head*)data.data();

    if(head->type==LOGIN_RES){
        LoginRes *loginRes = (LoginRes*)data.data();
        loginRes->flag = 1;
        if(loginRes->flag == 1){
            QMessageBox::information(this,"登录认证","登录成功");
            RealTimescreenScreenMainWindow * realtime = new RealTimescreenScreenMainWindow(this);
            realtime->show();
            this->close();
        }else{
            qDebug()<<"Login error";
            QMessageBox::information(this,"登录认证","登录失败,账户或者密码错误");
        }
    }
}
void Loginmainwindow::loginInit()
{
    /*QFile file("login.txt");
    if(file.open(QIODevice::ReadOnly|QIODevice::Text)){
        QTextStream in(&file);
        username = in.readLine();
        password = in.readLine();
        file.close();
    }
    ui->lineEdit_username->setText(username);
    ui->lineEdit_password->setText(password);*/
}
