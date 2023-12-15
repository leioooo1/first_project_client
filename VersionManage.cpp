#include "VersionManage.h"
#include <QFileDialog>
#include <QDebug>
#include <fstream>
#include <string>
#include <iostream>
#include <iomanip>
#include <regex>
#include <windows.h>
#include <QMessageBox>
#include <mysql.h>
using namespace std;

#define BUF_SIZE 1024

VersionManage::VersionManage(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
	//connect(ui.toolButton, SIGNAL(clicked()), this, SLOT(on_toolButton_clicked()));
	//connect(ui.pushButton_2, SIGNAL(clicked()), this, SLOT(on_pushButton_2_clicked()));
	ui.tableWidget->setColumnCount(8);
	ui.tableWidget->setHorizontalHeaderItem(0, new QTableWidgetItem(QString::fromLocal8Bit("ID")));
	ui.tableWidget->setHorizontalHeaderItem(1, new QTableWidgetItem(QString::fromLocal8Bit("文件名")));
	ui.tableWidget->setHorizontalHeaderItem(2, new QTableWidgetItem(QString::fromLocal8Bit("现有版本")));
	ui.tableWidget->setHorizontalHeaderItem(3, new QTableWidgetItem(QString::fromLocal8Bit("前版本")));
	ui.tableWidget->setHorizontalHeaderItem(4, new QTableWidgetItem(QString::fromLocal8Bit("客户端路径")));
	ui.tableWidget->setHorizontalHeaderItem(5, new QTableWidgetItem(QString::fromLocal8Bit("服务端路径")));
	ui.tableWidget->setHorizontalHeaderItem(6, new QTableWidgetItem(QString::fromLocal8Bit("MD5")));
	ui.tableWidget->setHorizontalHeaderItem(7, new QTableWidgetItem(QString::fromLocal8Bit("大小(单位：字节)")));
	ui.tableWidget->setShowGrid(true);
	ui.tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	//ui.tableWidget->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);

}

void VersionManage::on_toolButton_clicked()
{
	// 获取文件夹路径
	QFileDialog fileDialog;
	QString filepath = fileDialog.getOpenFileName(this,
		"Please choose a file",
		"C:\\");
	fileDialog.close();
	if (filepath.isEmpty()) {
		return;
	}
	else {
		ui.lineEdit->setText(filepath); //将文件路径显示在lineEdit控件中
	}
	
}

int row = 0;
static int count_i = 0;
int version = 1;
size_t VersionManage::getFileSize1(const char * fileName)
{
	if (fileName == NULL) {
		return 0;
	}

	// 这是一个存储文件(夹)信息的结构体，其中有文件大小和创建时间、访问时间、修改时间等
	struct stat statbuf;

	// 提供文件名字符串，获得文件属性结构体
	stat(fileName, &statbuf);

	// 获取文件大小
	size_t filesize = statbuf.st_size;

	return filesize;
}


void VersionManage::on_pushButton_2_clicked()
{
	if (ui.lineEdit->text() == NULL)
	{
		QMessageBox::critical(NULL, QString::fromLocal8Bit("错误!"), QString::fromLocal8Bit("error,请输入文件所在路径"));
		return;
	}
	int rowCount = ui.tableWidget->rowCount();

	//MD5加密
	MD5 md5;
	md5.reset();
	md5.update(ifstream(ui.lineEdit->text().toStdString()));
	ui.lineEdit_4->setText(QString::fromStdString(md5.toString()));

	//自动获取文件大小
	ui.lineEdit_3->setText(QString::number(getFileSize1(ui.lineEdit->text().toLatin1().constData())));

	
	//设置内容
	ui.tableWidget->insertRow(rowCount);
	ui.tableWidget->setItem(row,0,new QTableWidgetItem(QString::number(++count_i)));
	string filePath = ui.lineEdit->text().toStdString();
	std::regex fileRegex(".*/([^/]+)$");
	smatch match;
	if (std::regex_search(filePath, match, fileRegex)) {
		ui.tableWidget->setItem(row,1, new QTableWidgetItem(QString::fromStdString(match[1].str())));
	}
	ui.tableWidget->setItem(row, 2, new QTableWidgetItem(ui.comboBox->currentText()));
	ui.tableWidget->setItem(row, 3, new QTableWidgetItem(ui.comboBox_2->currentText()));
	ui.tableWidget->setItem(row, 4, new QTableWidgetItem(ui.lineEdit_2->text()));
	ui.tableWidget->setItem(row, 5, new QTableWidgetItem(ui.label_7->text()));
	ui.tableWidget->setItem(row, 6, new QTableWidgetItem(ui.lineEdit_4->text()));
	ui.tableWidget->setItem(row, 7, new QTableWidgetItem(ui.lineEdit_3->text()));
	row++;

}

void VersionManage::on_pushButton_clicked()
{

	std::string s_path = ui.lineEdit->text().toStdString();
	std::ifstream input_file(s_path, std::ios::binary);
	if (!input_file.is_open()) {
		cout << "Failed to open file." << endl;
	}
	FileData filedata;
	strcpy(filedata.filename, s_path.c_str());
	//filedata.filecontent[BUF_SIZE] = { 0 }; //缓冲区
	std::streampos resume_position = 0; // 设置默认为0，表示从头开始	
										// 如果需要断点续传，修改resume_position的值为已经上传的文件大小
	input_file.seekg(resume_position);
	while (!input_file.eof()) {
		// 读取文件数据
		input_file.read((char *)&filedata.filecontent, BUF_SIZE);
		int bytes_to_send = input_file.gcount();

		// 发送文件数据
		filedata.head.len = bytes_to_send;
		Myclient::getServer().myWrite((char*)&filedata, sizeof(FileData));

		resume_position += bytes_to_send;
	}

	input_file.close();

	for (int j = 0; j < ui.tableWidget->rowCount(); j++)
	{
		cout << ui.tableWidget->rowCount();
		//std::string file_path = ui.lineEdit->text().toStdString();
		std::string file_path = ui.tableWidget->item(j,1)->text().toStdString();
		std::regex slash_regex("/");
		std::string cmd_copy = "copy " + regex_replace(file_path, slash_regex, "\\") + " " +s_path;
		if (system(cmd_copy.c_str()) == 0) {
			QMessageBox::information(NULL, QString::fromLocal8Bit("提示!"), QString::fromLocal8Bit("文件已放到指定目录") , QMessageBox::Ok);
		}
		else {
			QMessageBox::critical(NULL, QString::fromLocal8Bit("错误!"), QString::fromLocal8Bit("error,系统找不到指定路径"));
		}

		string sql_value = "insert into version_information_table values(" +
			ui.tableWidget->item(j, 0)->text().toStdString() + ",'" +
			ui.tableWidget->item(j, 1)->text().toStdString() + "','" +
			ui.tableWidget->item(j, 2)->text().toStdString() + "','" +
			ui.tableWidget->item(j, 3)->text().toStdString() + "','" +
			ui.tableWidget->item(j, 4)->text().toStdString() + "','" +
			ui.tableWidget->item(j, 5)->text().toStdString() + "','" +
			ui.tableWidget->item(j, 6)->text().toStdString() + "','" +
			ui.tableWidget->item(j, 7)->text().toStdString() +
			"')";
		m_sql->RunSql(sql_value);
		
	}

}
