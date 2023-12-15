#include "permissionmanagementmodel.h"
#include "ui_permissionmanagementmodel.h"

#include <QMessageBox>
#pragma execution_character_set("utf-8")

PermissionManagementModel::PermissionManagementModel(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::PermissionManagementModel)
{
    ui->setupUi(this);
    //设置权限查询表格
    setPermissionInquiryForm();


    // 整行选择
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    connect(ui->tableView->selectionModel(), &QItemSelectionModel::currentRowChanged, this, &PermissionManagementModel::onRowClicked);

}

PermissionManagementModel::~PermissionManagementModel()
{
    delete ui;
}


int PermissionManagementModel::myTaskC(QByteArray data)
{
    Head * head = (Head*)data.data();
    qDebug()<<head->type;
    if(head->type == SEARCHPERMISSION_RES){
        searchPermissionRes * res = (searchPermissionRes*)data.data();
        qDebug()<<res->userName;
        if(res->state == 1){
            qDebug()<<"查询成功";
            // 创建一个新的 searchPermissionRes 对象

            searchPermissionRes newUser;
            qDebug()<<res->userName<<res->department;
            qstrncpy(newUser.userName, res->userName, sizeof(newUser.userName));
            qstrncpy(newUser.department, res->department, sizeof(newUser.department));
            newUser.per.admin      = res->per.admin;
            newUser.per.history    = res->per.history;
            newUser.per.warning    = res->per.warning;
            newUser.per.mapChange  = res->per.mapChange;
            newUser.per.logSearch  = res->per.logSearch;
            newUser.per.versionMan = res->per.versionMan;
            // 将新的 searchPermissionRes 对象添加到 m_userDataList 中
            m_userDataList.append(newUser);

            static int cnt = 0;
            ++cnt;
            if(cnt == res->cnt){
                for (int row = 0; row < m_userDataList.size(); ++row) {
                    const searchPermissionRes& userData = m_userDataList.at(row);
                    m_model->setItem(row, 0, new QStandardItem(userData.userName));
                    m_model->setItem(row, 1, new QStandardItem(userData.department));
                    m_model->setItem(row, 2, new QStandardItem(userData.per.admin ? "有权限" : "无权限"));
                    m_model->setItem(row, 3, new QStandardItem(userData.per.history ? "有权限" : "无权限"));
                    m_model->setItem(row, 4, new QStandardItem(userData.per.warning ? "有权限" : "无权限"));
                    m_model->setItem(row, 5, new QStandardItem(userData.per.mapChange ? "有权限" : "无权限"));
                    m_model->setItem(row, 6, new QStandardItem(userData.per.logSearch ? "有权限" : "无权限"));
                    m_model->setItem(row, 7, new QStandardItem(userData.per.versionMan ? "有权限" : "无权限"));
                }
                cnt = 0;
            }

            return 0;
        }
        else if(res->state == 2){
            QMessageBox::information(this,"查询失败","用户名错误");
            return -1;
        }
    }
    else if(head->type == UPDATEPERMISSION_RES){
        qDebug()<<"数据";
        updatePermissionRes * res = (updatePermissionRes*)data.data();
        qDebug()<<"更新成功";

        // 创建一个新的 updatePermissionRes 对象
        updatePermissionRes newUser;
        qDebug()<<res->userName<<res->department;
        qstrncpy(newUser.userName, res->userName, sizeof(newUser.userName));
        qstrncpy(newUser.department, res->department, sizeof(newUser.department));
        newUser.per.admin      = res->per.admin;
        newUser.per.history    = res->per.history;
        newUser.per.warning    = res->per.warning;
        newUser.per.mapChange  = res->per.mapChange;
        newUser.per.logSearch  = res->per.logSearch;
        newUser.per.versionMan = res->per.versionMan;

        m_model->setItem(m_currentRow, 0, new QStandardItem(newUser.userName));
        m_model->setItem(m_currentRow, 1, new QStandardItem(newUser.department));
        m_model->setItem(m_currentRow, 2, new QStandardItem(newUser.per.admin ? "有权限" : "无权限"));
        m_model->setItem(m_currentRow, 3, new QStandardItem(newUser.per.history ? "有权限" : "无权限"));
        m_model->setItem(m_currentRow, 4, new QStandardItem(newUser.per.warning ? "有权限" : "无权限"));
        m_model->setItem(m_currentRow, 5, new QStandardItem(newUser.per.mapChange ? "有权限" : "无权限"));
        m_model->setItem(m_currentRow, 6, new QStandardItem(newUser.per.logSearch ? "有权限" : "无权限"));
        m_model->setItem(m_currentRow, 7, new QStandardItem(newUser.per.versionMan ? "有权限" : "无权限"));
        return 0;
    }
    return -1;
}




void PermissionManagementModel::setPermissionInquiryForm()
{
    // 创建数据模型
    m_model = new QStandardItemModel(20, 8);

    // 设置水平表头
    m_model->setHorizontalHeaderItem(0, new QStandardItem("用户名"));
    m_model->setHorizontalHeaderItem(1, new QStandardItem("部门"));
    m_model->setHorizontalHeaderItem(2, new QStandardItem("权限管理"));
    m_model->setHorizontalHeaderItem(3, new QStandardItem("历史查询分析"));
    m_model->setHorizontalHeaderItem(4, new QStandardItem("警报提示及管理"));
    m_model->setHorizontalHeaderItem(5, new QStandardItem("地图标注定位"));
    m_model->setHorizontalHeaderItem(6, new QStandardItem("日志查看"));
    m_model->setHorizontalHeaderItem(7, new QStandardItem("版本管理"));

    // 视图
    ui->tableView->setModel(m_model);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    ui->tableView->show();
}

void PermissionManagementModel::on_pushButton_search_permission_clicked()
{
    // 清空旧数据
    m_userDataList.clear();
    QString userName = ui->lineEdit->text();
    QString department = ui->comboBox->currentText();
    qDebug()<<userName;
    qDebug()<<department;

    strcpy(m_searchPermission.userName,userName.toUtf8().data());
    strcpy(m_searchPermission.department,department.toUtf8().data());
    qDebug()<<m_searchPermission.userName;
    qDebug()<<m_searchPermission.department;


    Myclient::getServer().myWrite((char*)&m_searchPermission, m_searchPermission.head.len);
}

void PermissionManagementModel::onRowClicked(const QModelIndex index)
{
    if (index.isValid()) {
        idx = index;
        m_currentRow = index.row();

        // 获取用户名并转换为 char 数组
        QString userName = m_model->data(m_model->index(index.row(), 0)).toString();
        QByteArray userNameByteArray = userName.toUtf8();
        qstrncpy(m_userDataUpdate.userName, userNameByteArray.constData(), sizeof(m_userDataUpdate.userName));

        // 获取部门并转换为 char 数组
        QString department = m_model->data(m_model->index(index.row(), 1)).toString();
        QByteArray departmentByteArray = department.toUtf8();
        qstrncpy(m_userDataUpdate.department, departmentByteArray.constData(), sizeof(m_userDataUpdate.department));

        ui->checkBox->setChecked(m_model->data(m_model->index(index.row(), 2), Qt::DisplayRole).toString() == "有权限" ? true : false);
        ui->checkBox_2->setChecked(m_model->data(m_model->index(index.row(), 3), Qt::DisplayRole).toString() == "有权限" ? true : false);
        ui->checkBox_3->setChecked(m_model->data(m_model->index(index.row(), 4), Qt::DisplayRole).toString() == "有权限" ? true : false);
        ui->checkBox_4->setChecked(m_model->data(m_model->index(index.row(), 5), Qt::DisplayRole).toString() == "有权限" ? true : false);
        ui->checkBox_5->setChecked(m_model->data(m_model->index(index.row(), 6), Qt::DisplayRole).toString() == "有权限" ? true : false);
        ui->checkBox_6->setChecked(m_model->data(m_model->index(index.row(), 7), Qt::DisplayRole).toString() == "有权限" ? true : false);
    }
}

void PermissionManagementModel::on_pushButton_update_permission_clicked()
{
    if(idx.isValid()){
        QString userName = m_model->data(m_model->index(idx.row(), 0)).toString();
        if (!userName.isEmpty()) {
        m_userDataUpdate.per.admin      = ui->checkBox  ->isChecked();
        m_userDataUpdate.per.history    = ui->checkBox_2->isChecked();
        m_userDataUpdate.per.warning    = ui->checkBox_3->isChecked();
        m_userDataUpdate.per.mapChange  = ui->checkBox_4->isChecked();
        m_userDataUpdate.per.logSearch  = ui->checkBox_5->isChecked();
        m_userDataUpdate.per.versionMan = ui->checkBox_6->isChecked();

        Myclient::getServer().myWrite((char*)&m_userDataUpdate,m_userDataUpdate.head.len);
        qDebug()<<"更新";
        }
   }
}
