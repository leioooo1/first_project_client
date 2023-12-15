#include "warningmainwindow2.h"
#include "ui_warningmainwindow2.h"

WarningMainWindow2::WarningMainWindow2(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::WarningMainWindow2)
{
    ui->setupUi(this);
}

WarningMainWindow2::~WarningMainWindow2()
{
    delete ui;
}

void WarningMainWindow2::on_pushButton_clicked()
{
 emit this->getMaxTem(ui->comboBox->currentText());
 emit this->getMinTem(ui->comboBox_2->currentText());
 emit this->getMaxHum(ui->comboBox_3->currentText());
 emit this->getMinHum(ui->comboBox_4->currentText());
 emit this->getMaxLig(ui->comboBox_5->currentText());
 emit this->getMinLig(ui->comboBox_6->currentText());
 emit this->getMaxSta(ui->comboBox_7->currentText());
 emit this->getMinSta(ui->comboBox_8->currentText());
 emit this->getMaxDus(ui->comboBox_9->currentText());
 emit this->getMinDus(ui->comboBox_10->currentText());
}

void WarningMainWindow2::on_pushButton_6_clicked()
{
   this->hide();
}
