//@timae  : 2022.8.25 - 2022.8.29
//@author : gonzalez
//@func   : 使用无向图实现

//绘制图片然后在场景中进行实现

#include <QtWidgets/QApplication>
#include "matrixGraph.h"           // qt ui界面类
#include "inputWindow.h"

using namespace std;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    matrixGraph w;
    w.show();
    return a.exec();
}
