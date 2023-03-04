//@time   : 2022.9.2
//@author : gonzalez
//@func   : myQGraphicsView执行函数
#include "myQGraphicsView.h"
#include <cmath>
#include <QDebug>
#include <qmath.h>
#include <QWheelEvent>
#include <qscrollbar.h>

// @name   : MyQGraphicsView
// @time   : 2022.9.2
// @func   : 构造函数
myQGraphicsView::myQGraphicsView(QWidget* parent) : QGraphicsView(parent)
{

}
// @name   : wheelEvent
// @time   : 2022.9.2
// @func   : 重写wheelEvent 进行缩放
void myQGraphicsView::wheelEvent(QWheelEvent* event)
{
	auto step = event->angleDelta().y();
	QPointF cursorPoint = event->position(); 
	QPointF scenePos = this->mapToScene(cursorPoint.x(), cursorPoint.y()); 
	qreal factor = 1 + ((qreal)step / (qreal)viewport()->height()); scale(factor, factor);
	
	QPointF viewPoint = this->transform().map(scenePos); 
	horizontalScrollBar()->setValue(viewPoint.x() - cursorPoint.x()); 
	verticalScrollBar()->setValue(viewPoint.y() - cursorPoint.y());

}
// @name   : shrink
// @time   : 2022.9.2
// @func   : 
void myQGraphicsView::shrink()
{
    zoom(1.0/1.2);
}
// @name   : expand
// @time   : 2022.9.2
// @func   : 
void myQGraphicsView::expand()
{
    zoom(1.2);
}
// @name   : zoom
// @time   : 2022.9.2
// @func   : 
void myQGraphicsView::zoom(double scaleFactor)
{
    //缩放函数
    qreal factor = transform().scale(scaleFactor, scaleFactor).mapRect(QRectF(0, 0, 1, 1)).width();
    if (factor < 0.1 || factor > 50)
        return;
    scale(scaleFactor, scaleFactor);
}
// @name   : refresh
// @time   : 2022.9.2
// @func   : 刷新
void myQGraphicsView::refresh()
{
	this->viewport()->update();
}


