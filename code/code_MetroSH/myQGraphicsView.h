//@time   : 2022.9.2
//@author : gonzalez
//@func   : 提升为myQGraphicsView
#pragma once
#ifndef MYQGRAPHICSVIEW_H
#define MYQGRAPHICSVIEW_H

#include <QGraphicsView>

class myQGraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit myQGraphicsView(QWidget* parent = nullptr);
    void wheelEvent(QWheelEvent* event);                  // 重写鼠标缩放
    void expand();                                        // 放大
    void shrink();                                        // 缩小
    void refresh();                                       // 刷新

private:
    void zoom(double);                                    // 尺寸变化

signals:

};

#endif // MYQGRAPHICSVIEW_H