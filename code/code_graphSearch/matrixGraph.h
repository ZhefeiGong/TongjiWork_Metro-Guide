//@timae  : 2022.8.25 - 2022.8.29
//@author : gonzalez
//@func   : 可视化头文件

#pragma once

#include <QtWidgets/QMainWindow>
#include <qstring.h>
#include <qpoint.h>
#include <qtimer.h>         // 定时器类
#include "ui_matrixGraph.h" // 包含主页面UI
#include "inputWindow.h"    // 包含子页面类
#include "algorithm.h"      // 包含内部算法类


#define NONINPUT   0
#define ALLINPUT   1
#define EACHINPUT  2
#define RADIUS     25
#define TIMEINTER  1000
#define LITS_SHOW  0

/***************************************** matrixGraph *****************************************/
//@name : matrixGraph类
//@func : 总界面函数
//@time : 2022.8.26
class matrixGraph : public QMainWindow
{
    Q_OBJECT

public:
    matrixGraph(QWidget *parent = nullptr);                // 构造函数
    ~matrixGraph();                                        // 析构函数

    void backDeal(QString data);                           // 处理输入的Qstring数据
    bool Str2Algorithm(char* str);                         // 根据参数形式对内部数据进行初始化
    bool siteGenerate(void);                               // 求出图的随机位置
    bool drawMatrix(int* chooseV = NULL, int numV = 0);    // 已经选择的结点画成红色
    bool drawList(void);                                   // 画出邻接列表
    bool drawStack(void);                                  // 画出栈
    bool drawQueue(void);                                  // 画出队列

private:

    int inputMethod;                 // 输入的模型
    Ui::matrixGraphClass* ui;        // 主界面ui界面创建
    inputWindow* inputPage;          // 输入界面创建
    matrixGraphAlgorithm* algorithm; // 内部算法数据

    int func;                        // 功能显示判断

    QTimer* timerDfs;                // DFS定时器类
    string recordDfs;                // 可视化记录
    int numDfs;                      // 可视化记录
    QTimer* timerBfs;                // BFS定时器类
    string recordBfs;                // 可视化记录
    int numBfs;                      // 可视化记录
    QPoint* vertexSite;              // 图的坐标(随机生成)

};


/***************************************** 工具函数 *****************************************/
void splitString(vector<string>& strings, const string& org_string, const string& seq);
