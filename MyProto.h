#pragma once
#include <vector>

using namespace std;

enum TypeInfo {

    HEART_CHECK_REQ,   // 心跳检测请求
    HEART_CHECK_RES,   // 心跳检测响应
    TEST_REQ, // 测试请求
    TEST_RES, // 测试响应
    LOGIN_REQ, // 登录请求
    LOGIN_RES, // 登录响应
    //权限
    SEARCHPERMISSION_REQ,
    SEARCHPERMISSION_RES,
    UPDATEPERMISSION_REQ,
    UPDATEPERMISSION_RES,
    //大屏显示
    MATERIAl_REQ,
    MATERIAl_RES,
    ATTENDANCE_REQ,
    ATTENDANCE_RES,
    WORKSHOPDEVICE_REQ,
    WORKSHOPDEVICE_RES,
    //历史查询
    HISTORICAL_QUERY_REQ, // 历史信息查询请求包
    HISTORICAL_QUERY_RES, // 历史信息查询响应包
    //日志查询
    SEARCHLOG_REQ,
    SEARCHLOG_RES,
    //地图显示
    MAP_ADD,//数据库增
    MAP_DELETE,//数据库删
    MAP_SEARCH,//数据库查
    MAP_RES,//响应
    //版本更新
    VER_NUM_REQ,
    VER_NUM_RES,
    UP_VER_NUM_REQ,
    UP_VER_NUM_RES,
    FILE_REQ,
    FILE_RES,
    //文件上传
    OWN_UP_VER_NUM_REQ,
    OWN_FILE_REQ,
    OWN_OK,
    OWN_OKK

};

struct Head
{
    int type;
    int len;
};

struct LoginReq
{
    Head head;
    char username[16];
    char password[16];
    LoginReq() {
        head.type = LOGIN_REQ;
        head.len = sizeof(LoginReq);
    }
};
struct LoginRes
{
    Head head;
    bool flag;//标志是否成功
    LoginRes() {
        head.type = LOGIN_RES;
        head.len = sizeof(LoginRes);
    }
};



struct HeartCheckReq
{
    Head head;
    HeartCheckReq() {
        head.type = HEART_CHECK_REQ;
        head.len = sizeof(HeartCheckReq);
    }
};

struct HeartCheckRes
{
    Head head;
    HeartCheckRes() {
        head.type = HEART_CHECK_RES;
        head.len = sizeof(HeartCheckRes);
    }
};

struct DataMaterial {
	int used;
	int total;
};
struct MaterialREQ {
	Head head;
	MaterialREQ() {
		head.type = MATERIAl_REQ;
		head.len = sizeof(MaterialREQ);
	}
}; 
struct MaterialRES {
	Head head;
	DataMaterial data[5];
	MaterialRES() {
		head.type = MATERIAl_RES;
		head.len = sizeof(MaterialRES);
	}
}; // 原材料


struct DataAttendance {
	int practicalCnt; // 实际人数
	int shoudComeCnt; // 应到人数
	char workshop[20]; // 车间名称
};
struct AttendanceREQ {
	Head head;
	AttendanceREQ() {
		head.type = ATTENDANCE_REQ;
		head.len = sizeof(AttendanceREQ);
	}
}; 
struct AttendanceRES {
	Head head;
	DataAttendance attendance[4];
	AttendanceRES() {
		head.type = ATTENDANCE_RES;
		head.len = sizeof(AttendanceRES);
	}
}; // 上班人数



struct DataWorkshopDevice
{
	double	Temperature; // 温度
	double	Humidity;	// 湿度
	double	Dust;		// 灰尘
	double	Light;		// 光感
	double	Noise;		//噪音
	char workshop[20];	// 车间名
};
struct WorkshopDeviceREQ {
	Head head;
	WorkshopDeviceREQ() {
		head.type = WORKSHOPDEVICE_REQ;
		head.len = sizeof(WorkshopDeviceREQ);
	}
}; 
struct WorkshopDeviceRES {
	Head head;
    DataWorkshopDevice workshopDevice[4];
	WorkshopDeviceRES() {
		head.type = WORKSHOPDEVICE_RES;
		head.len = sizeof(WorkshopDeviceRES);
	}
}; // 车间设备


// 历史信息请求包
struct HistoricalReq
{
    Head head;
    char startDate[32];
    char endDate[32];
    HistoricalReq() {
        head.type = HISTORICAL_QUERY_REQ;
        head.len = sizeof(HistoricalReq);
    }
};

struct DataHistory
{
    int id;             //id
    char p_date[32];    // 日期
    int p_count;        // 产品总数
    int p_yield;        // 良品数
    int p_damage;       // 损坏数
};
// 历史信息响应包
struct HistoricalRes
{
    Head head;
    int cnt = 0;
    DataHistory data[0];
    HistoricalRes() {
        head.type = HISTORICAL_QUERY_RES;
        head.len = sizeof(HistoricalRes);
    }
};


struct searchLogReq
{
    Head head;
    char logType[32];
    char beginTime[32];
    char endTime[32];
    searchLogReq() {
        head.type = SEARCHLOG_REQ;
        head.len = sizeof(searchLogReq);
    }
};

struct LogInfo{
    char time[20];
    int processId;
    int threadId;
    char fileName[30];
    char functionName[20];
    char logInfomation[200];
};

struct searchLogRes
{
    Head head;
    int cnt;
    LogInfo info[0];
    searchLogRes() {
        head.type = SEARCHLOG_RES;
        head.len = sizeof(searchLogRes);
    }
};



struct Permission
{
    bool admin;
    bool history;
    bool warning;
    bool mapChange;
    bool logSearch;
    bool versionMan;
};

struct searchPermissionReq
{
    Head head;
    char userName[32];
    char department[32];
    searchPermissionReq() {
        head.type = SEARCHPERMISSION_REQ;
        head.len = sizeof(searchPermissionReq);
    }

};

struct searchPermissionRes
{
    Head head;
    int cnt;
    int state;
    char userName[32];
    char department[32];
    Permission per;
    searchPermissionRes(){
        head.type = SEARCHPERMISSION_RES;
        head.len = sizeof(searchPermissionRes);
    }

};

struct updatePermissionReq
{
    Head head;
    char userName[32];
    char department[32];
    Permission per;
    updatePermissionReq(){
        head.type = UPDATEPERMISSION_REQ;
        head.len = sizeof(updatePermissionReq);
    }

};

struct updatePermissionRes
{
    Head head;
    int state;
    char userName[32];
    char department[32];
    Permission per;
    updatePermissionRes(){
        head.type = UPDATEPERMISSION_RES;
        head.len = sizeof(updatePermissionRes);
    }
};


struct MapAdd
{
    Head head;
    char data[32];
    MapAdd() {
        head.type = MAP_ADD;
        head.len = sizeof(MapAdd);
    }
};

struct MapDelete
{
    Head head;
    int markerNumber;
    MapDelete() {
        head.type = MAP_DELETE;
        head.len = sizeof(MapDelete);
    }
};

struct MapSearch
{
    Head head;
    MapSearch() {
        head.type = MAP_SEARCH;
        head.len = sizeof(MapSearch);
    }
};

struct MapRes//qt收包
{
    Head head;
    char data[32];
    int number;
    MapRes() {
        head.type = MAP_RES;
        head.len = sizeof(MapRes);
    }
};



//对比版本号请求包
struct VerNumReq{
    Head head;
    char ver_num[32];
    VerNumReq(){
        head.type = VER_NUM_REQ;//版本号请求
        head.len = sizeof(VerNumReq);
    }
};

//对比版本号回应包
struct VerNumRes{
    Head head;
    char ver_num[32];
    VerNumRes(){
        head.type = VER_NUM_RES;//版本号回应
        head.len = sizeof(VerNumRes);
    }
};

//升级版本号请求包
struct UpVerNumReq {
    Head head;
    char up_ver_num[32];//存 version1.1
    UpVerNumReq() {
        head.type = UP_VER_NUM_REQ;//升级版本号请求
        head.len = sizeof(UpVerNumReq);
    }
};
//数据库内容发送信息包
struct FileMessage {
    Head head;
    int id;//ID
    char filename[256]; // 文件名
    char c_ver[32];//现有版本
    char p_ver[32];//前版本
    char c_path[50];//客户端路径
    char p_path[50];//服务端路径
    char md5[32];//MD5值
    char filesize[32]; // 文件大小
    FileMessage()
    {
        head.type = UP_VER_NUM_RES;//升级版本号回应
        head.len = sizeof(FileMessage);
    }
};


struct FileReq{
    Head head;
    char filename[32];
    FileReq(){
        head.type = FILE_REQ;//版本号请求
        head.len = sizeof(FileReq);
    }
};

struct FileData{
    Head head;
    char filename[256]; // 文件名
    char filecontent[1024]; // 文件内容
    FileData(){
        head.type = FILE_RES;//版本号请求
        head.len = sizeof(FileData);
    }
};

