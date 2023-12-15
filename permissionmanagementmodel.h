#ifndef PERMISSIONMANAGEMENTMODEL_H
#define PERMISSIONMANAGEMENTMODEL_H

#include <QMainWindow>
#include <QStandardItemModel>
#include <QTableView>


#include "myclient.h"
#include "MyProto.h"

namespace Ui {
class PermissionManagementModel;
}

class PermissionManagementModel : public QMainWindow
{
    Q_OBJECT


public:
    explicit PermissionManagementModel(QWidget *parent = 0);
    ~PermissionManagementModel();
    friend class PermissionBusiness;
public slots:
    int myTaskC(QByteArray data);
private:
    void setPermissionInquiryForm();

private slots:
    void onRowClicked(const QModelIndex index);

    void on_pushButton_search_permission_clicked();

    void on_pushButton_update_permission_clicked();


private:
    Ui::PermissionManagementModel *ui;
    int m_currentRow;
    //QTcpSocket * m_client;
    QStandardItemModel *m_model;
    QList<searchPermissionRes> m_userDataList;
    searchPermissionReq m_searchPermission;
    updatePermissionReq m_userDataUpdate;
    updatePermissionRes m_updateData;
    QModelIndex idx;
};

#endif // PERMISSIONMANAGEMENTMODEL_H
