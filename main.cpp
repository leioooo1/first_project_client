#include "Loginmainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Loginmainwindow w;
    w.show();

    return a.exec();
}
