#ifndef DELWIDGET_H
#define DELWIDGET_H

#include <QWidget>

namespace Ui {
class DelWidget;
}

class DelWidget : public QWidget
{
    Q_OBJECT

public:
    explicit DelWidget(QWidget *parent = 0);
    ~DelWidget();
signals:
    void mysignal(int);
private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::DelWidget *ui;
};

#endif // DELWIDGET_H
