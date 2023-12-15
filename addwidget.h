#ifndef ADDWIDGET_H
#define ADDWIDGET_H

#include <QWidget>

namespace Ui {
class AddWidget;
}

class AddWidget : public QWidget
{
    Q_OBJECT

public:
    explicit AddWidget(QWidget *parent = 0);
    ~AddWidget();
signals:
    void mysignal(QString);
private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::AddWidget *ui;
};

#endif // ADDWIDGET_H
