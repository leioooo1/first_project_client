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
	ui.tableWidget->setHorizontalHeaderItem(1, new QTableWidgetItem(QString::fromLocal8Bit("�ļ���")));
	ui.tableWidget->setHorizontalHeaderItem(2, new QTableWidgetItem(QString::fromLocal8Bit("���а汾")));
	ui.tableWidget->setHorizontalHeaderItem(3, new QTableWidgetItem(QString::fromLocal8Bit("ǰ�汾")));
	ui.tableWidget->setHorizontalHeaderItem(4, new QTableWidgetItem(QString::fromLocal8Bit("�ͻ���·��")));
	ui.tableWidget->setHorizontalHeaderItem(5, new QTableWidgetItem(QString::fromLocal8Bit("�����·��")));
	ui.tableWidget->setHorizontalHeaderItem(6, new QTableWidgetItem(QString::fromLocal8Bit("MD5")));
	ui.tableWidget->setHorizontalHeaderItem(7, new QTableWidgetItem(QString::fromLocal8Bit("��С(��λ���ֽ�)")));
	ui.tableWidget->setShowGrid(true);
	ui.tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	//ui.tableWidget->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);

}

void VersionManage::on_toolButton_clicked()
{
	// ��ȡ�ļ���·��
	QFileDialog fileDialog;
	QString filepath = fileDialog.getOpenFileName(this,
		"Please choose a file",
		"C:\\");
	fileDialog.close();
	if (filepath.isEmpty()) {
		return;
	}
	else {
		ui.lineEdit->setText(filepath); //���ļ�·����ʾ��lineEdit�ؼ���
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

	// ����һ���洢�ļ�(��)��Ϣ�Ľṹ�壬�������ļ���С�ʹ���ʱ�䡢����ʱ�䡢�޸�ʱ���
	struct stat statbuf;

	// �ṩ�ļ����ַ���������ļ����Խṹ��
	stat(fileName, &statbuf);

	// ��ȡ�ļ���С
	size_t filesize = statbuf.st_size;

	return filesize;
}


void VersionManage::on_pushButton_2_clicked()
{
	if (ui.lineEdit->text() == NULL)
	{
		QMessageBox::critical(NULL, QString::fromLocal8Bit("����!"), QString::fromLocal8Bit("error,�������ļ�����·��"));
		return;
	}
	int rowCount = ui.tableWidget->rowCount();

	//MD5����
	MD5 md5;
	md5.reset();
	md5.update(ifstream(ui.lineEdit->text().toStdString()));
	ui.lineEdit_4->setText(QString::fromStdString(md5.toString()));

	//�Զ���ȡ�ļ���С
	ui.lineEdit_3->setText(QString::number(getFileSize1(ui.lineEdit->text().toLatin1().constData())));

	
	//��������
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
	//filedata.filecontent[BUF_SIZE] = { 0 }; //������
	std::streampos resume_position = 0; // ����Ĭ��Ϊ0����ʾ��ͷ��ʼ	
										// �����Ҫ�ϵ��������޸�resume_position��ֵΪ�Ѿ��ϴ����ļ���С
	input_file.seekg(resume_position);
	while (!input_file.eof()) {
		// ��ȡ�ļ�����
		input_file.read((char *)&filedata.filecontent, BUF_SIZE);
		int bytes_to_send = input_file.gcount();

		// �����ļ�����
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
			QMessageBox::information(NULL, QString::fromLocal8Bit("��ʾ!"), QString::fromLocal8Bit("�ļ��ѷŵ�ָ��Ŀ¼") , QMessageBox::Ok);
		}
		else {
			QMessageBox::critical(NULL, QString::fromLocal8Bit("����!"), QString::fromLocal8Bit("error,ϵͳ�Ҳ���ָ��·��"));
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
