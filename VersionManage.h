#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_VersionManage.h"
#include "md5.h"
#include <sys/stat.h>
#include <WinSock2.h>
#include <map>
#include <mutex>
#include "struct_data.h"
#include <thread>
#include "MysqlManager.h"
#include "myclient.h"
using namespace std;
#pragma comment(lib,"ws2_32.lib") // º”‘ÿæ≤Ã¨ø‚

class VersionManage : public QMainWindow
{
    Q_OBJECT

public:
    VersionManage(QWidget *parent = Q_NULLPTR);
	size_t getFileSize1(const char *fileName);
signals:
	void fileSent();
private slots:
void on_toolButton_clicked();
void on_pushButton_2_clicked();
void on_pushButton_clicked();

private:
    Ui::VersionManageClass ui;
	MysqlManager *m_sql;
};
