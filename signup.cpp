#include "signup.h"
#include "ui_signup.h"
#include "Loginmainwindow.h"

Signup::Signup(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Signup)
{
    ui->setupUi(this);
    //去窗口边框
           setWindowFlags(Qt::FramelessWindowHint | windowFlags());
    QPixmap *pix = new QPixmap(":/蚂蚁logo.png");
    QSize sz = ui->label_image->size();
    ui->label_image->setPixmap(pix->scaled(sz));
    //combobox下拉框样式表生效


}

Signup::~Signup()
{
    delete ui;
}

//返回登录按钮
void Signup::on_btn_return_clicked()
{
    Loginmainwindow *w = new Loginmainwindow;
    w->show();
    this->close();
}

//注册按钮
void Signup::on_pushButton_2_clicked()
{
//    sqlite_Init();
//    QString username = ui->lineEdit_username->text();
//    QString password = ui->lineEdit_passwd->text();
//    QString surepass = ui->lineEdit_surepasswd->text();
//    //判断密码是否一致
//    if(password == surepass)
//    {
//        QString sql=QString("insert into user(username,password) values('%1','%2');")
//                .arg(username).arg(password);
//        //创建执行语句对象
//        QSqlQuery query;
//        //判断执行结果
//        if(!query.exec(sql))
//        {
//            qDebug()<<"insert into error";
//            QMessageBox::information(this,"注册认证","插入失败！");
//        }
//        else
//        {
//            qDebug()<<"insert into success";
//            QMessageBox::information(this,"注册认证","插入成功！");
//            Loginmainwindow *w = new Loginmainwindow;
//            w->show();
//            this->close();
//        }

//    }else{
//        QMessageBox::information(this,"注册认证","两次密码输入不一致");
//    }
}

void Signup::on__toolButton_min_clicked()
{
    showMinimized();
}

void Signup::on_toolButton_max_clicked()
{
    showMaximized();
}

void Signup::on_toolButton_close_clicked()
{
    close();
}
