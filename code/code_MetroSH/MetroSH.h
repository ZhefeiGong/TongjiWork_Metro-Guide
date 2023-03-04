//@time   : 2022.8.29
//@author : gonzalez
//@func   : 主页面头文件

#pragma once

#include <QtWidgets/QMainWindow>
#include <qgraphicsscene.h>
#include "ui_MetroSH.h"
#include "myQGraphicsView.h"
#include "algorithm.h"
#include "stationPage.h"
#include "routePage.h"

#define SCENESPACE  500
#define STATIONCHANGE 1
#define ROUTESCHANGE  2

class MetroSH : public QMainWindow
{
    Q_OBJECT

public:
    MetroSH(QWidget *parent = nullptr);
    ~MetroSH();
    void closeEvent(QCloseEvent* event);            // 重载closeEvent事件

private:
    Ui::MetroSHClass* ui;                           // 主页面
    subwayAlgorithm internal;                       // 内部实现
    QGraphicsScene* scene;                          // 绘图场景
    stationPage* pageST;                            // 站点修改页面
    routePage* pageRO;                              // 路线修改页面

    QRectF sceneRect;                               // 绘图场景获取
    unordered_map<string, QPoint>coordHashMap;      // 站点坐标哈希表

    void sceneInitiate(void);                                      // Scene画面信息初始化
    bool drawStation(string name);                                 // 画出对应站点
    bool drawEdge(string stationA, string stationB, QColor color); // 画出连线
    bool drawGraph(void);                                          // 画出图形
    double calcuateDis(string stationA, string StationB);          // 计算两个站点之间的距离
    bool drawAns(vector<string> ans);                              // 绘制并显示结果路线

    void restart(void);                                            // 重置
    QLabel* currentTimeLabel;                                      // 时间存储
    QTimer* timer;                                                 // 计时器
    void timeUpdate(void);                                         // 状态栏时间更新

    void searchProcedure(void);                     // 路线搜寻过程判断
    bool isArrivible(string site1, string site2);   // 两点是否可抵达

    void transmit(int method);                      // 站点传输
    void receive(int method);                       // 站点接受

signals:
    void clearGraph(void);                          // 清除界面信号
};
