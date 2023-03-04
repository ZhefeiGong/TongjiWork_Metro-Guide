//@time   : 2022.8.29
//@author : gonzalez
//@func   : 上海地铁PROJECT主函数

#include "MetroSH.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MetroSH w;
    w.show();
    return a.exec();
}
