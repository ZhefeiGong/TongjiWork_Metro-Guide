//@time   : 2022.9.3
//@author : gonzalez
//@func   : 添加路线页面头文件
#pragma once

#include <QtWidgets/QMainWindow>
#include <qgraphicsscene.h>
#include "ui_routePage.h"
#include "myQGraphicsView.h"
#include "algorithm.h"


class routePage : public QMainWindow
{
    Q_OBJECT

public:
    routePage(QWidget* parent = nullptr);
    ~routePage();

    //----------------- 数据传递 -----------------
    vector<routeInfo> routes;                       // 路线信息
    vector<siteInfo> sites;                         // 站点信息的邻接列表
    vector<lineInfo> lines;                         // 路线信息的邻接列表
    unordered_map<string, int>routesHashMap;        // 路线名字与index的哈希表
    unordered_map<string, int>sitesHashMap;         // 站点名字与index的哈希表
    unordered_map<string, int>linesHashMap;         // 路线名字与index的哈希表

    QRectF sceneRect;                               // 绘图场景获取
    unordered_map<string, QPoint>coordHashMap;      // 站点坐标哈希表

    void initiate(void);                            // 根据传入参数初始化

private:

    Ui::routePageClass* ui;                // 主页面
    QGraphicsScene* scene;                 // 绘图场景

    QColor newColor;                       // 新添加的路线颜色

    double longiMin = 120.51;              // 经度MIN
    double longiMax = 122.12;              // 经度MAX
    double latiMin = 30.40;                // 纬度MIN
    double latiMax = 31.53;                // 纬度MAX

    bool addRouteDeal(void);               // 处理添加
    bool addRouteTextDeal(void);           // 处理添加(text)
    bool deleteRouteDeal(void);            // 处理删除

    // 绘图函数
    bool drawStation(string name);
    bool drawEdge(string stationA, string stationB, QColor color);
    double calcuateDis(string stationA, string stationB);
    void drawRoute(string name);



signals:
    void clearGraph(void);                 // 清除页面信号
    void back(int method);                 // 数据反馈

};