#-------------------------------------------------
#
# Project created by QtCreator 2023-12-02T10:22:41
#
#-------------------------------------------------
QT       += charts
QT       += core gui network multimedia webenginewidgets webchannel

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = real_time_screen
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        realtimescreenscreenmainwindow.cpp \
    showview.cpp \
    myclient.cpp \
    historicalquerymainwindow.cpp \
    setmainwindow.cpp \
    logquerymodel.cpp \
    permissionmanagementmodel.cpp \
    arrangedwnd.cpp \
    notifymanager.cpp \
    notifywnd.cpp \
    warningmainwindow1.cpp \
    warningmainwindow2.cpp \
    renewalmainwindow.cpp \
    signup.cpp \
    Loginmainwindow.cpp \
    mapwidget.cpp \
    delwidget.cpp \
    channel.cpp \
    addwidget.cpp



HEADERS += \
        realtimescreenscreenmainwindow.h \
    showview.h \
    MyProto.h \
    myclient.h \
    historicalquerymainwindow.h \
    setmainwindow.h \
    logquerymodel.h \
    permissionmanagementmodel.h \
    arrangedwnd.h \
    notifymanager.h \
    notifywnd.h \
    warningmainwindow1.h \
    warningmainwindow2.h \
    renewalmainwindow.h \
    signup.h \
    Loginmainwindow.h \
    mapwidget.h \
    delwidget.h \
    channel.h \
    addwidget.h



FORMS += \
        realtimescreenscreenmainwindow.ui \
    historicalquerymainwindow.ui \
    mainwindow.ui \
    logquerymodel.ui \
    permissionmanagementmodel.ui \
    warningmainwindow1.ui \
    warningmainwindow2.ui \
    renewalmainwindow.ui \
    signup.ui \
    Loginmainwindow.ui \
    mapwidget.ui \
    delwidget.ui \
    addwidget.ui



RESOURCES += \
    media.qrc
  # pic.qrc
