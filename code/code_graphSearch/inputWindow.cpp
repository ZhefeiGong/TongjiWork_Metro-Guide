//@timae  : 2022.8.27 - 2022.8.29
//@author : gonzalez
//@func   : 可视化执行文件

#include "matrixGraph.h"
#include "inputWindow.h"
#include <qpushbutton.h>
#include <qtextedit.h>

/***************************************** inputWindow *****************************************/
//@name : inputWindow
//@func : 构造函数
//@time : 2022.8.27
inputWindow::inputWindow(QWidget* parent) : QMainWindow(parent)
{
	this->ui = new Ui::inputWindowClass;
	this->ui->setupUi(this);

	// 确定按钮槽
	connect(this->ui->confirmBtn, &QPushButton::clicked, [=]() {
		QString data = this->ui->inputText->toPlainText();
		this->ui->inputText->clear();
		emit this->back(data);// 发送返回信号
		});
	// 重写按钮槽
	connect(this->ui->againBtn, &QPushButton::clicked, this->ui->inputText, &QTextEdit::clear);

}
//@name : ~inputWindow
//@func : 析构函数
//@time : 2022.8.27
inputWindow::~inputWindow()
{
	delete ui;
}
//@name : noticeUpdate
//@func : 更新界面Label提示函数
//@time : 2022.8.27
void inputWindow::noticeUpdate(QString temp)
{
	this->ui->noticeText->setText(temp);
	return;
}
