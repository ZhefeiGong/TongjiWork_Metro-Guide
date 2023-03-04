//@time   : 2022.9.3
//@author : gonzalez
//@func   : 添加站点执行函数

#include "stationPage.h"
#include <qgraphicsitem.h>
#include <qmessagebox.h>

// @name   : stationPage
// @time   : 2022.9.2
// @func   : 构造函数
#define STATIONNONE "NULL"
#define STATIONCHANGE 1
stationPage::stationPage(QWidget* parent) : QMainWindow(parent)
{
	this->ui = new Ui::stationPageClass;
	this->ui->setupUi(this);

	// stationPage页面scene初始化
	this->scene = new QGraphicsScene;
	this->scene->setSceneRect(this->sceneRect);
	this->ui->graphicsView->setRenderHint(QPainter::Antialiasing);     // 精致绘图
	this->ui->graphicsView->setScene(this->scene);                     // 添加绘图场景
	this->ui->graphicsView->setDragMode(QGraphicsView::ScrollHandDrag);// 设置鼠标拖拽格式

    //------------ 添加注意初始化 ------------
    QString noticeAdd = "😊原有线路基础上增添站点，具体步骤如下:\n";
    noticeAdd += "    1.下拉框选择需要增添站点的线路\n";
    noticeAdd += "    2.选择需要添加的站点位置，站点选择需连续\n";
    noticeAdd += "    3.填入增添的站点名、经度、纬度等信息\n";
    noticeAdd += "    4.点击确认添加按钮即可\n";
    noticeAdd += "🙂需要注意的是:\n";
    noticeAdd += "    a.选择路线后，可在左侧鼠标滚轮缩放查看细节\n";
    noticeAdd += "    b.两站之间增添站点才有意义，故前驱后继需连续\n";
    noticeAdd += "    c.若表示在队首/队尾前后增添站点，另一个位置设置为NULL即可\n";
    noticeAdd += "    d.若增添站点名在该路线中已存在，则不可增添\n";
    noticeAdd += "    e.若增添站点名在该系统中已存在，则可选择采用现有站点作为公有站点增添；否则重新输入站点名\n";
    noticeAdd += "    f.由于系统以上海为背景，因此 经度需在120.51-122.12内 | 纬度需在30.40-31.53内\n";

    this->ui->textBrowserAdd->setText(noticeAdd);


    //------------ 删除注意初始化 ------------
    QString noticeDelete = "😊原有线路基础上删除站点，具体步骤如下:\n";
    noticeDelete += "    1.下拉框选择删除站点所在路线\n";
    noticeDelete += "    2.下拉框选择该站点\n";
    noticeDelete += "    3.点击确认删除，删除对应站点\n";
    noticeDelete += "🙂需要注意的是:\n";
    noticeDelete += "    a.选择路线后，可在左侧鼠标滚轮缩放查看细节\n";
    noticeDelete += "    b.若删除结点是多个路线相交点，此次删除仅能删除所选路线上该点记录，其他路线仍保留\n";
    
    this->ui->textBrowserDelete->setText(noticeDelete);

    // 添加选择框
    connect(this->ui->comboBoxRoute, &QComboBox::activated, [=]() {
        if (this->ui->tabWidget->currentIndex() == 0) {
            // 页面显示
            QString get = this->ui->comboBoxRoute->currentText();
            string currentGet = (string)(get.toLocal8Bit());
            int index = this->routesHashMap.find(currentGet)->second;
            this->drawRoute(this->routes[index].getName());
            // 下拉框补充
            this->ui->comboBoxBefore->clear();
            this->ui->comboBoxAfter->clear();
            this->ui->comboBoxBefore->addItem(STATIONNONE);
            this->ui->comboBoxAfter->addItem(STATIONNONE);
            for (int i = 0; i < this->routes[index].getNum(); i++) {
                this->ui->comboBoxBefore->addItem(QString::fromLocal8Bit(this->routes[index].getSite(i).name.c_str()));
                this->ui->comboBoxAfter->addItem(QString::fromLocal8Bit(this->routes[index].getSite(i).name.c_str()));
            }
        }
        });

    // 删除选择框
    connect(this->ui->comboBoxRoute_delete, &QComboBox::activated, [=]() {
        if (this->ui->tabWidget->currentIndex() == 1) {
            // 页面显示
            QString get = this->ui->comboBoxRoute_delete->currentText();
            string currentGet = (string)(get.toLocal8Bit());
            int index = this->routesHashMap.find(currentGet)->second;
            this->drawRoute(this->routes[index].getName());
            // 下拉框补充
            this->ui->comboBoxStation_delete->clear();
            for (int i = 0; i < this->routes[index].getNum(); i++) {
                this->ui->comboBoxStation_delete->addItem(QString::fromLocal8Bit(this->routes[index].getSite(i).name.c_str()));
            }
        }
        });

    // 清除界面信息
    void(stationPage:: * clearSign)(void) = &stationPage::clearGraph;
    connect(this, clearSign, this->scene, &QGraphicsScene::clear);

    // 添加站点
    connect(this->ui->pushButtonAdd, &QPushButton::clicked, [=]() {
        if (this->addStation()) {
            
            // 发送更改信号
            emit this->back(STATIONCHANGE);
            // 展现新的页面情况
            QString get = this->ui->comboBoxRoute->currentText();
            string currentGet = (string)(get.toLocal8Bit());
            int index = this->routesHashMap.find(currentGet)->second;
            this->drawRoute(this->routes[index].getName());
            // 清除历史记录
            this->ui->comboBoxBefore->clear();
            this->ui->comboBoxAfter->clear();
            for (int i = 0; i < this->routes[index].getNum(); i++) {
                this->ui->comboBoxBefore->addItem(QString::fromLocal8Bit(this->routes[index].getSite(i).name.c_str()));
                this->ui->comboBoxAfter->addItem(QString::fromLocal8Bit(this->routes[index].getSite(i).name.c_str()));
            }
            this->ui->lineEditName->clear();
            this->ui->lineEditLongi->clear();
            this->ui->lineEditLati->clear();
            // 给出提示
            QMessageBox::information(this, "SUCCESS", "添加成功");

        }
        });

    // 删除站点
    connect(this->ui->pushButtonDelete, &QPushButton::clicked, [=]() {
        if (this->deleteStation()) {

            // 发送更改信号
            emit this->back(STATIONCHANGE);
            // 展现新的页面情况
            QString get = this->ui->comboBoxRoute_delete->currentText();
            string currentGet = (string)(get.toLocal8Bit());
            int index = this->routesHashMap.find(currentGet)->second;
            this->drawRoute(this->routes[index].getName());
            // 清除历史记录
            this->ui->comboBoxStation_delete->clear();
            for (int i = 0; i < this->routes[index].getNum(); i++) {
                this->ui->comboBoxStation_delete->addItem(QString::fromLocal8Bit(this->routes[index].getSite(i).name.c_str()));
            }
            // 给出提示
            QMessageBox::information(this, "SUCCESS", "删除成功");

        }
        });

    // 状态栏
    QLabel* author = new QLabel("Copyright 2022 gonzalez");
    this->ui->statusbar->addWidget(author);
    QLabel* project = new QLabel("地铁指南");
    this->ui->statusbar->addPermanentWidget(project);

}
// @name   : stationPage
// @time   : 2022.9.2
// @func   : 析构函数
stationPage::~stationPage()
{


}
// @name   : initiate
// @time   : 2022.9.3
// @func   : 根据输入参数初始化操作
void stationPage::initiate(void)
{
    // 界面清除
    emit this->clearGraph();

    // comboBox初始化
    this->ui->comboBoxRoute->clear();
    this->ui->comboBoxRoute_delete->clear();
    for (int i = 0; i < this->routes.size(); i++) {
        this->ui->comboBoxRoute->addItem(QString::fromLocal8Bit(this->routes[i].getName().c_str()));
        this->ui->comboBoxRoute_delete->addItem(QString::fromLocal8Bit(this->routes[i].getName().c_str()));
    }

}
// @name   : addStation
// @time   : 2022.9.3
// @func   : 添加站点
bool stationPage::addStation(void)
{
    string lineName = (string)(this->ui->comboBoxRoute->currentText().toLocal8Bit());
    string staBefore = (string)(this->ui->comboBoxBefore->currentText().toLocal8Bit());
    string staAfter = (string)(this->ui->comboBoxAfter->currentText().toLocal8Bit());

    int index = this->routesHashMap.find(lineName)->second;

    string name = (string)(this->ui->lineEditName->text().toLocal8Bit());
    double longi = this->ui->lineEditLongi->text().toDouble();
    double lati = this->ui->lineEditLati->text().toDouble();

    // 路线名
    if (lineName.size() == 0) {
        QMessageBox::warning(this, "提示", "请选择路线");
        return false;
    }
    // 邻接站点
    if (staBefore == STATIONNONE && staAfter == STATIONNONE) {
        QMessageBox::warning(this, "提示", "请至少选择一个邻近站点");
        return false;
    }
    // 首为NULL
    if (staBefore == STATIONNONE) {
        if (staAfter != this->routes[index].getSite(0).name && staAfter != this->routes[index].getSite(this->routes[index].getNum() - 1).name) {
            QMessageBox::warning(this, "提示", "当存在一个NULL时\n另一个邻接点须为端点");
            return false;
        }
    }
    // 尾为NULL
    if (staAfter == STATIONNONE) {
        if (staBefore != this->routes[index].getSite(0).name && staBefore != this->routes[index].getSite(this->routes[index].getNum() - 1).name) {
            QMessageBox::warning(this, "提示", "当存在一个NULL时\n另一个邻接点须为端点");
            return false;
        }
	}
    // 首尾均不为NULL
    if (staBefore != STATIONNONE && staAfter != STATIONNONE) {
        int indexBefore = 0;
        int indexAfter = 0;
        for (int i = 0; i < this->routes[index].getNum(); i++) {
            if (staBefore == this->routes[index].getSite(i).name) {
                indexBefore = i;
            }
            else if (staAfter == this->routes[index].getSite(i).name) {
                indexAfter = i;
            }
        }
        if (abs(indexBefore - indexAfter) != 1) {
            QMessageBox::warning(this, "提示", "两个邻接点需要相邻");
            return false;
        }
    }
    // 名字为空
    if (name.size() == 0) {
        QMessageBox::warning(this, "提示", "名字不能为空");
        return false;
    }
    // 该路线中站点重名
    for (int i = 0; i < this-> routes[index].getNum(); i++) {
        if (this->routes[index].getSite(i).name == name) {
            QMessageBox::warning(this, "提示", "与该路线中<" + QString::fromLocal8Bit(name.c_str()) + ">站点重名");
            return false;
        }
    }
    // 所有站点重名
    if (this->sitesHashMap.find(name) != this->sitesHashMap.end()) {
        if (QMessageBox::Yes == QMessageBox::question(this, "注意", "系统中存在重复站名<" + QString::fromLocal8Bit(name.c_str()) + ">\n是否将该点替换为已有站点?\n(反之，请重新输入)", QMessageBox::Yes | QMessageBox::No)) {
            longi = this->sites[this->sitesHashMap.find(name)->second].longitude;
            lati = this->sites[this->sitesHashMap.find(name)->second].latitude;
            this->ui->lineEditLati->clear();
            this->ui->lineEditLongi->clear();
            this->ui->lineEditLongi->setText(QString::number(longi, 'f', 7));
            this->ui->lineEditLati->setText(QString::number(lati, 'f', 7));
        }
        else {
            return false;
        }
    }

    // 经度
    if (longi<this->longiMin || longi>this->longiMax) {
        QMessageBox::warning(this, "提示", "站点经度超范围");
        return false;
    }
    // 纬度
    if (lati<this->latiMin || lati>this->latiMax) {
        QMessageBox::warning(this, "提示", "站点纬度超范围");
        return false;
    }

    // 添加站点
    routeSiteInfo site;
    site.name = name;
    site.latitude = lati;
    site.longitude = longi;
    this->routes[index].insertSite(site, staBefore, staAfter);

    return true;

}
// @name   : deleteStation
// @time   : 2022.9.3
// @func   : 删除站点
bool stationPage::deleteStation(void)
{
    string lineName = (string)(this->ui->comboBoxRoute_delete->currentText().toLocal8Bit());
    string stationName = (string)(this->ui->comboBoxStation_delete->currentText().toLocal8Bit());
    if (lineName.size() == 0) {
        QMessageBox::warning(this, "提示", "请选择路线");
        return false;
    }
    if (stationName.size() == 0) {
        QMessageBox::warning(this, "提示", "请选择站点");
        return false;
    }
    // 删除该站点
    int index = this->routesHashMap.find(lineName)->second;
    this->routes[index].deleteSite(stationName);

    return true;
}


/****************************************** 画图函数 ******************************************/
// @name   : drawStation
// @time   : 2022.9.2
// @func   : 画出对应的站点
#define NODERADIUS 4
#define DEFAULTCOLOR 0x000000
bool stationPage::drawStation(string name)
{
    QPoint coord = this->coordHashMap.find(name)->second;                                 // 坐标信息
    siteInfo site = this->sites[this->sitesHashMap.find(name)->second]; // 站点信息

    // 画出该点
    QGraphicsEllipseItem* stationItem = new QGraphicsEllipseItem;
    // 提示信息
    QString tip = "站名 : " + QString::fromLocal8Bit(site.name.c_str()) + "\n" +
        "经纬度 : " + QString::number(site.longitude, 'f', 7) + " " + QString::number(site.latitude, 'f', 7) + "\n" +
        "站点路线 : ";
    for (int i = 0; i < site.viaLines.size(); i++) {
        tip += QString::fromLocal8Bit(site.viaLines[i].c_str()) + " ";
    }
    tip += "\n";
    tip += "相邻站点\n";
    for (int i = 0; i < site.adjacentNodes.size(); i++) {
        tip += QString::fromLocal8Bit(site.adjacentNodes[i].c_str()) + " : " + QString::number(this->calcuateDis(site.name, site.adjacentNodes[i]), 'f', 7) + "km" + "\n";
    }
    // 颜色
    QColor siteColor;
    if (site.viaLines.size() != 1) {
        siteColor = QColor(QRgb(DEFAULTCOLOR));
    }
    else {
        int index = this->routesHashMap.find(site.viaLines[0])->second;
        siteColor = this->routes[index].getColor();
    }

    stationItem->setRect(-NODERADIUS, -NODERADIUS, NODERADIUS << 1, NODERADIUS << 1); // 设置站点圆形区域
    stationItem->setPos(coord);                                                       // 设置中心坐标
    stationItem->setPen(siteColor);                                                   // 设置画笔颜色
    stationItem->setBrush(QColor(QRgb(0xffffff)));                                    // 设置填充
    stationItem->setCursor(Qt::PointingHandCursor);                                   // 设置鼠标形态
    stationItem->setToolTip(tip);                                                     // 设置提示信息
    this->scene->addItem(stationItem);

    // 添加站点名字
    QGraphicsTextItem* textItem = new QGraphicsTextItem;
    textItem->setPlainText(QString::fromLocal8Bit(site.name.c_str()));                // 设置站点名字
    textItem->setFont(QFont("consolas", 5, 1));                                       // 设置名字格式
    textItem->setPos(coord.x(), coord.y() - NODERADIUS * 2);                          // 设置名字位置
    this->scene->addItem(textItem);

    return true;
}
// @name   : drawEdge
// @time   : 2022.9.3
// @func   : 链接对应的站点
#define LINE_WIDTH  2
bool stationPage::drawEdge(string stationA, string stationB, QColor color)
{
    QPoint coordA = this->coordHashMap.find(stationA)->second;
    QPoint coordB = this->coordHashMap.find(stationB)->second;
    siteInfo siteA = this->sites[this->sitesHashMap.find(stationA)->second]; // 站点信息
    siteInfo siteB = this->sites[this->sitesHashMap.find(stationB)->second]; // 站点信息


    vector<string> commonLine;
    for (int i = 0; i < siteA.viaLines.size(); i++) {
        for (int j = 0; j < siteB.viaLines.size(); j++) {
            if (siteA.viaLines[i] == siteB.viaLines[j]) {
                commonLine.push_back(siteA.viaLines[i]);
            }
        }
    }
    QString tip = "途径 : " + QString::fromLocal8Bit(siteA.name.c_str()) + " " + QString::fromLocal8Bit(siteB.name.c_str()) + "\n";
    tip += "距离 : " + QString::number(this->calcuateDis(stationA, stationB), 'f', 7) + "km" + "\n";
    tip += "线路 : \n";
    for (int i = 0; i < commonLine.size(); i++) {
        int index = this->routesHashMap.find(commonLine[i])->second;
        QString origin = QString::fromLocal8Bit(this->routes[index].getOrigin().c_str());
        QString destination = QString::fromLocal8Bit(this->routes[index].getDestination().c_str());
        tip += QString::fromLocal8Bit(commonLine[i].c_str()) + " [" + origin + "<->" + destination + "]\n";
    }

    QGraphicsLineItem* edgeItem = new QGraphicsLineItem;
    edgeItem->setPen(QPen(color, LINE_WIDTH));
    edgeItem->setCursor(Qt::PointingHandCursor); // 设置光标
    edgeItem->setToolTip(tip);                   // 添加注解
    edgeItem->setLine(QLineF(coordA, coordB));   // 画出对应的线

    this->scene->addItem(edgeItem);
    return true;

}
// @name   : calcuateDis
// @time   : 2022.9.3
// @func   : 计算两个站点之间距离
double stationPage::calcuateDis(string stationA, string stationB)
{
    double longi_A = this->sites[this->sitesHashMap.find(stationA)->second].longitude;
    double lati_A = this->sites[this->sitesHashMap.find(stationA)->second].latitude;
    double longi_B = this->sites[this->sitesHashMap.find(stationB)->second].longitude;
    double lati_B = this->sites[this->sitesHashMap.find(stationB)->second].latitude;

    double distance = sqrt(pow((longi_A - longi_B), 2) + pow((lati_A - lati_B), 2)) * 111;
    return distance;
}
// @name   : drawLine
// @time   : 2022.9.3
// @func   : 画出一条线路
void stationPage::drawRoute(string name)
{
    // 界面清除
    emit this->clearGraph();
    // 初始化
    int index = this->routesHashMap.find(name)->second;
    QColor color = this->routes[index].getColor();
    // 画线
    for (int i = 0; i < this->routes[index].getNum() - 1; i++) {
        string name1 = this->routes[index].getSite(i).name;
        string name2 = this->routes[index].getSite(i + 1).name;
        this->drawEdge(name1, name2, color);
    }
    // 画点
    for (int i = 0; i < this->routes[index].getNum(); i++) {
        string name = this->routes[index].getSite(i).name;
        this->drawStation(name);
    }

    return;
}