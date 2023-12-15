#include "addwidget.h"
#include "ui_addwidget.h"

AddWidget::AddWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AddWidget)
{
    ui->setupUi(this);
}

AddWidget::~AddWidget()
{
    delete ui;
}

void AddWidget::on_pushButton_clicked()
{
    QString str = ui->lineEdit->text();
    emit mysignal(str);
}

void AddWidget::on_pushButton_2_clicked()
{
    ui->lineEdit->clear();
    this->hide();
}
