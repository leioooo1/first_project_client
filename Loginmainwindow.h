#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include <QtWidgets/QWidget>
#include <QMainWindow>

#include <QMessageBox>//消息盒子
#include <QDebug>
#include <QTextCodec>
#include <QSettings>

void sqlite_Init();

// QT_BEGIN_NAMESPACE
namespace Ui { class Loginmainwindow; }
// QT_END_NAMESPACE

class Loginmainwindow : public QMainWindow
{
    Q_OBJECT

public:
    Loginmainwindow(QWidget *parent = nullptr);
    ~Loginmainwindow();
    void setMarginWidth(const int &iWidth);           //设置鼠标可以在界面边缘多大范围内拖动改变界面大小

    void serResizable(bool bCanResize);                    //设置是否可以拖动改变大小
protected:
    void mouseMoveEvent(QMouseEvent *e);//鼠标移动
    void mousePressEvent(QMouseEvent *e);//鼠标按下移动
private slots:
    void on_btn_signin_clicked();

    void on_btn_signup_clicked();

    void on_toolButton_min_clicked();

    void on_toolButton_close_clicked();

    void on_toolButton_max_clicked();

    void on_radioButton_password_clicked();

    void LoginWindow(QByteArray);
private:
    void loginInit();//登录初始化
private:
    Ui::Loginmainwindow *ui;
    QPoint p;
    bool ispassword;
    QString username; // 存储用户名
    QString password; // 存储密码
    bool rememberMe; // 存储是否记住密码
};
#endif // MAINWINDOW_H
