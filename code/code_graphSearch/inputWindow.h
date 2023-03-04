#pragma once
//@timae  : 2022.8.27 - 2022.8.29
//@author : gonzalez
//@func   : 输入可视化头文件

#include <QtWidgets/QMainWindow>
#include "qstring.h"
#include "ui_inputWindow.h"


/***************************************** inputWindow *****************************************/
//@name : inputWindow类
//@func : 输入界面类
//@time : 2022.8.26
class inputWindow : public QMainWindow
{
    Q_OBJECT

public:
    inputWindow(QWidget* parent = nullptr);
    ~inputWindow();
    void noticeUpdate(QString temp); // 提示更新外部函数 
signals:
    void back(QString data);         // 输入信息返回信号
private:
    Ui::inputWindowClass* ui;        // ui界面创建
};


