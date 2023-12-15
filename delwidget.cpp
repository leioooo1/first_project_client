#include "delwidget.h"
#include "ui_delwidget.h"

DelWidget::DelWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DelWidget)
{
    ui->setupUi(this);
}

DelWidget::~DelWidget()
{
    delete ui;
}

void DelWidget::on_pushButton_clicked()
{
    QString str = ui->lineEdit->text();
    emit mysignal(str.toInt());
}

void DelWidget::on_pushButton_2_clicked()
{
    ui->lineEdit->clear();
    this->hide();
}
