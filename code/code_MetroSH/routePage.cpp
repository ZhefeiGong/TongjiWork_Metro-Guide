//@time   : 2022.9.3
//@author : gonzalez
//@func   : 路线执行函数

#include "routePage.h"
#include <qcolordialog.h>
#include <qdebug.h>
#include <qpalette.h>
#include <qgraphicsitem.h>
#include <qmessagebox.h>
#include <sstream>

#define ROUTECHANGE 2

// @name   : stationPage
// @time   : 2022.9.2
// @func   : 构造函数
routePage::routePage(QWidget* parent) : QMainWindow(parent)
{
	this->ui = new Ui::routePageClass;
	this->ui->setupUi(this);

	// 页面初始化
	this->scene = new QGraphicsScene;
	this->scene->setSceneRect(this->sceneRect);
	this->ui->graphicsView->setRenderHint(QPainter::Antialiasing);     // 精致绘图
	this->ui->graphicsView->setScene(this->scene);                     // 添加绘图场景
	this->ui->graphicsView->setDragMode(QGraphicsView::ScrollHandDrag);// 设置鼠标拖拽格式


	//------------ 添加注意初始化 ------------
    QString noticeAdd = "😊原有线路基础上增添路线，具体步骤如下:\n";
    noticeAdd += "    1.输入增添的路线名\n";
    noticeAdd += "    2.设置路线颜色(点击设置颜色按钮，对话框选择颜色)\n";
    noticeAdd += "    3.设置起点信息(名字/经度/纬度)\n";
    noticeAdd += "    4.设置起点信息(名字/经度/纬度)\n";
    noticeAdd += "    5.点击确认添加即可\n";
    noticeAdd += "🙂需要注意的是:\n";
    noticeAdd += "    a.路线名不可与现有路线名重复\n";
    noticeAdd += "    b.若起点(终点)名在该系统中已存在，则可选择采用现有站点作为起点(终点)；否则重新输入站点名\n";
    noticeAdd += "    c.由于系统以上海为背景，因此 经度需在120.51-122.12内 | 纬度需在30.40-31.53内\n";
    noticeAdd += "    d.此处仅设置路线起点/终点，其他站点可在<站点管理>中进行增添\n";
    noticeAdd += "    e.添加成功后，左侧将出现添加路线信息，可使用鼠标滚轮缩放查看\n";

    this->ui->textBrowserAddRoute->setText(noticeAdd);

	//------------ 删除注意初始化 ------------
	QString noticeAddText = "😊原有线路基础上增添路线(通过文本形式)，具体步骤如下:\n";
    noticeAddText += "    1.按照对应格式输入路线数据\n";
	noticeAddText += "    2.点击确认添加即可\n";
	noticeAddText += "🙂输入格式如下(具体可参照src/data/demo.txt):\n";
	noticeAddText += "    name 1号线\n";
	noticeAddText += "    numSite 3\n";
	noticeAddText += "    colour 0xE70012\n";
	noticeAddText += "    fromTo 富锦路 宝安公路\n";
	noticeAddText += "    富锦路 31.3948771 121.4197766\n";
	noticeAddText += "    友谊西路 31.3837478 121.4231229\n";
	noticeAddText += "    宝安公路 31.3721781 121.4261394\n";
    noticeAddText += "    ----------\n";
    noticeAddText += "    name 2号线\n";
	noticeAddText += "    numSite 4\n";
	noticeAddText += "    colour 0xE70012\n";
	noticeAddText += "    fromTo 衡山路 上海南站\n";
	noticeAddText += "    衡山路 31.2076988 121.4429212\n";
	noticeAddText += "    徐家汇 31.1950824 121.4338887\n";
	noticeAddText += "    上海体育馆 31.1838665 121.4317564\n";
	noticeAddText += "    漕宝路 31.1702714 121.4303461\n";
	noticeAddText += "    上海南站 31.1556231 121.4271388\n";
	noticeAddText += "    ==========\n";
    noticeAddText += "😴需要注意的是:\n";
    noticeAddText += "    a.此处一条路线可以添加多个站点\n";
    noticeAddText += "    b.此处可以一次性添加多条路线\n";
    noticeAddText += "    c.添加路线名不可与现有的重复\n";
    noticeAddText += "    d.若站点与现有重复，可以使用现有站点，或修改站点名解决\n";
    noticeAddText += "    e.添加成功后，左侧将出现添加路线信息，可使用鼠标滚轮缩放查看\n";
    noticeAddText += "    f.由于系统以上海为背景，因此 经度需在120.51-122.12内 | 纬度需在30.40-31.53内\n";
    this->ui->textBrowserAddRouteText->setText(noticeAddText);

	//------------ 删除注意初始化 ------------
	QString noticeDelete = "😊原有线路基础上删除路线，具体步骤如下:\n";
    noticeDelete += "    1.选择需要删除的路线名\n";
    noticeDelete += "    2.点击确认删除即可\n";
    noticeDelete += "🙂需要注意的是:\n";
    noticeDelete += "    a.选择路线后可在左侧鼠标滚路缩放查看路线细节\n";

    this->ui->textBrowserDelete->setText(noticeDelete);

	// 清除scene界面
	connect(this, &routePage::clearGraph, this->scene, &QGraphicsScene::clear);

	//-------------------- 添加路线页面 --------------------
	// 设置颜色
	this->ui->AddRoute_lineColor->setReadOnly(true); //设置颜色对话框不可编辑
	connect(this->ui->pushButtonSetColor, &QPushButton::clicked, [=]() {
		QColorDialog dialog;

		connect(&dialog, &QColorDialog::colorSelected, [=](auto& color) {
			this->newColor = color;
			this->ui->AddRoute_lineColor->setStyleSheet(QString("background-color: rgb(%1,%2,%3)").arg(this->newColor.red()).arg(this->newColor.green()).arg(this->newColor.blue()));		
			});
		dialog.exec();
		});
	// 确定输入
	connect(this->ui->pushButtonAddRoute, &QPushButton::clicked, this, &routePage::addRouteDeal);

	//-------------------- 添加路线页面(text) --------------------
	connect(this->ui->pushButtonAddRouteText, &QPushButton::clicked, this, &routePage::addRouteTextDeal);

	//-------------------- 删除路线页面(界面) --------------------
    connect(this->ui->comboBoxDelete, &QComboBox::activated, [=]() {
        if (this->ui->tabWidget->currentIndex() == 2) {
            // 页面显示
            QString get = this->ui->comboBoxDelete->currentText();
            string currentGet = (string)(get.toLocal8Bit());
            int index = this->routesHashMap.find(currentGet)->second;
            emit this->clearGraph();
            this->drawRoute(this->routes[index].getName());

        }
        });
    connect(this->ui->pushButtonDelete, &QPushButton::clicked, this, &routePage::deleteRouteDeal);

    // 状态栏
    QLabel* author = new QLabel("Copyright 2022 gonzalez");
    this->ui->statusbar->addWidget(author);
    QLabel* project = new QLabel("地铁指南");
    this->ui->statusbar->addPermanentWidget(project);

}
// @name   : stationPage
// @time   : 2022.9.2
// @func   : 析构函数
routePage::~routePage()
{
}
// @name   : initiate
// @time   : 2022.9.4
// @func   : 参数初始化操作
void routePage::initiate(void)
{
	// 清除界面
	emit this->clearGraph();

	// 添加下拉框
    this->ui->comboBoxDelete->clear();
	for (int i = 0; i < this->routes.size(); i++) {
		this->ui->comboBoxDelete->addItem(QString::fromLocal8Bit(this->routes[i].getName().c_str()));
	}

}
// @name   : addRouteDeal
// @time   : 2022.9.4
// @func   : 添加路线
bool routePage::addRouteDeal(void)
{
    // 数据获取
    string routeName = (string)(this->ui->AddRoute_lineName->text().toLocal8Bit());
    
    string startName = (string)(this->ui->AddRoute_startName->text().toLocal8Bit());
    double startLongi = this->ui->AddRoute_startLongi->text().toDouble();
    double startLati = this->ui->AddRoute_startLati->text().toDouble();
    
    string endName = (string)(this->ui->AddRoute_endName->text().toLocal8Bit());
    double endLongi = this->ui->AddRoute_endLongi->text().toDouble();
    double endLati = this->ui->AddRoute_endLati->text().toDouble();

    // 路线名
    if (routeName.size() == 0) {
        QMessageBox::warning(this, "提示", "请输入路线名");
        return false;
    }
    for (int i = 0; i < this->routes.size(); i++) {
        if (this->routes[i].getName() == routeName) {
            QMessageBox::warning(this, "提示", "路线重名 请重新输入");
            return false;
        }
    }
    // 颜色
    if (!this->newColor.isValid()) {
        QMessageBox::warning(this, "提示", "请选择颜色");
        return false;
    }
    // 起点名
    if (startName.size() == 0) {
        QMessageBox::warning(this, "提示", "请输入起点");
        return false;
    }
    if (this->sitesHashMap.find(startName) != this->sitesHashMap.end()) {
        if (QMessageBox::Yes == QMessageBox::question(this, "注意", "存在重复站名<" + QString::fromLocal8Bit(startName.c_str()) + ">\n是否将起点替换为已有站点?\n(反之，请重新输入)", QMessageBox::Yes | QMessageBox::No)) {
            startLongi = this->sites[this->sitesHashMap.find(startName)->second].longitude;
            startLati = this->sites[this->sitesHashMap.find(startName)->second].latitude;
            this->ui->AddRoute_startLongi->clear();
            this->ui->AddRoute_startLati->clear();
            this->ui->AddRoute_startLongi->setText(QString::number(startLongi, 'f', 7));
            this->ui->AddRoute_startLati->setText(QString::number(startLati, 'f', 7));
        }
        else {
            return false;
        }
    }
    // 终点名
    if (startName.size() == 0) {
        QMessageBox::warning(this, "提示", "请输入终点");
        return false;
    }
    if (this->sitesHashMap.find(endName) != this->sitesHashMap.end()) {
        if (QMessageBox::Yes == QMessageBox::question(this, "注意", "存在重复站名<" + QString::fromLocal8Bit(endName.c_str()) + ">\n是否将终点替换为已有站点?\n(反之，请重新输入)", QMessageBox::Yes | QMessageBox::No)) {
            endLongi = this->sites[this->sitesHashMap.find(endName)->second].longitude;
            endLati = this->sites[this->sitesHashMap.find(endName)->second].latitude;
            this->ui->AddRoute_endLongi->clear();
            this->ui->AddRoute_endLati->clear();
            this->ui->AddRoute_endLongi->setText(QString::number(endLongi, 'f', 7));
            this->ui->AddRoute_endLati->setText(QString::number(endLati, 'f', 7));
        }
        else {
            return false;
        }
    }
    // 起点经纬度
    if (startLongi<this->longiMin || startLongi>this->longiMax) {
        QMessageBox::warning(this, "提示", "起点经度超范围");
        return false;
    }
    if (startLati<this->latiMin || startLati>this->latiMax) {
        QMessageBox::warning(this, "提示", "起点纬度超范围");
        return false;
    }
    // 终点经纬度
    if (endLongi<this->longiMin || endLongi>this->longiMax) {
        QMessageBox::warning(this, "提示", "终点经度超范围");
        return false;
    }
    if (endLati<this->latiMin || endLati>this->latiMax) {
        QMessageBox::warning(this, "提示", "终点纬度超范围");
        return false;
    }

    // route添加
    routeSiteInfo start;
    start.name = startName;
    start.longitude = startLongi;
    start.latitude = startLati;
    routeSiteInfo end;
    end.name = endName;
    end.longitude = endLongi;
    end.latitude = endLati;

    routeInfo route;
    route.setId(this->routes[this->routes.size() - 1].getId() + 1);
    route.setNum(2);
    route.setColor(this->newColor);
    route.setName(routeName);
    route.setOrigin(startName);
    route.setDestination(endName);
    route.pushSite(start);
    route.pushSite(end);
    this->routes.push_back(route);

    // 发送更改信号
    emit this->back(ROUTECHANGE);

    // 页面更新
    this->ui->AddRoute_endLati->clear();
    this->ui->AddRoute_endLongi->clear();
    this->ui->AddRoute_endName->clear();
    this->ui->AddRoute_lineColor->clear();
    this->ui->AddRoute_lineName->clear();
    this->ui->AddRoute_startLati->clear();
    this->ui->AddRoute_startLongi->clear();
    this->ui->AddRoute_startName->clear();
    this->ui->comboBoxDelete->clear();
    for (int i = 0; i < this->routes.size(); i++) {
        this->ui->comboBoxDelete->addItem(QString::fromLocal8Bit(this->routes[i].getName().c_str()));
    }

    // 展现新的页面情况
    emit this->clearGraph();
    this->drawRoute(routeName);

    // 给出提示
    QMessageBox::information(this, "SUCCESS", "添加成功");

    return true;
}
// @name   : addRouteTextDeal
// @time   : 2022.9.4
// @func   : 添加路线(TEXT)
bool routePage::addRouteTextDeal(void)
{
    // 进行数据读入
    string dataInput = (string)(this->ui->textEditAddRouteText->toPlainText().toLocal8Bit());

    // 数据提取
    stringstream sstream(dataInput);
    string name_ = "name";
    string numSite_ = "numSite";
    string colour_ = "colour";
    string fromTo_ = "fromTo";
    string continueSign = "----------";
    string endSign = "==========";

    int idCount = this->routes[this->routes.size() - 1].getId() + 1;
    vector<string>addRoutesName;

    bool urgentOut = false;
    while (sstream) {
        // route初始化
        routeInfo route;
        route.setId(idCount);
        idCount++;
        int num;
        string temp, name, origin, destination;
        // name获取
        sstream >> temp;
        if (temp == name_) {
            sstream >> name;
            if (this->routesHashMap.find(name) != this->routesHashMap.end()) {
                QMessageBox::warning(this, "提示", "路线<"+QString::fromLocal8Bit(name.c_str()) + ">重名\n请重新输入");
                return false;
            }
            route.setName(name);
        }
        else {
            QMessageBox::critical(this, "警告", "数据格式错误");
            return false;
        }
        // numSite获取
        sstream >> temp;
        if (temp == numSite_) {
            sstream >> num;
            route.setNum(num);
        }
        else {
            QMessageBox::critical(this, "警告", "数据格式错误");
            return false;
        }
        // color初始化
        sstream >> temp;
        if (temp == colour_) {
            sstream >> temp;
            QString  str = QString::fromStdString(temp);
            int value = str.toInt(nullptr, 16);
            route.setColor(QColor(QRgb(value)));
        }
        else {
            QMessageBox::critical(this, "警告", "数据格式错误");
            return false;
        }
        // fromTo获取
        sstream >> temp;
        if (temp == fromTo_) {
            sstream >> origin;
            sstream >> destination;
            route.setOrigin(origin);
            route.setDestination(destination);
        }
        else {
            QMessageBox::critical(this, "警告", "数据格式错误");
            return false;
        }
        // 获取站点信息
        for (int i = 0; i < route.getNum(); i++) {
            routeSiteInfo site;
            sstream >> site.name;
            sstream >> site.latitude;
            sstream >> site.longitude;
            // 存在重复站名
            if (this->sitesHashMap.find(site.name) != this->sitesHashMap.end()) {
                if (QMessageBox::Yes == QMessageBox::question(this, "注意", QString::fromLocal8Bit(route.getName().c_str()) + "中存在重复站名<" + QString::fromLocal8Bit(site.name.c_str()) + ">\n是否将该点替换为已有站点?\n(反之，请重新输入)", QMessageBox::Yes | QMessageBox::No)) {
                    site.longitude = this->sites[this->sitesHashMap.find(site.name)->second].longitude;
                    site.latitude = this->sites[this->sitesHashMap.find(site.name)->second].latitude;
                }
                else {
                    if (addRoutesName.size() != 0) {
                        // 保存已存入数据
                        if (QMessageBox::Save == QMessageBox::question(this, "注意", "已存入路线是否保留?", QMessageBox::Save | QMessageBox::Discard)) {
                            urgentOut = true;
                            break;
                        }
                        // 丢弃已存入数据
                        else {
                            for (int i = 0; i < addRoutesName.size(); i++) {
                                this->routes.pop_back();
                                this->routesHashMap.erase(addRoutesName[i]);
                            }
                            return false;
                        }
                    }
                    else {
                        return false;
                    }
                }
            }
            // 经度范围判断
            if (site.longitude<this->longiMin || site.longitude>this->longiMax) {
                QMessageBox::warning(this, "提示", "<" + QString::fromLocal8Bit(site.name.c_str()) + ">经度超范围");
                return false;
            }
            // 纬度范围判断
            if (site.latitude<this->latiMin || site.latitude>this->latiMax) {
                QMessageBox::warning(this, "提示", "<" + QString::fromLocal8Bit(site.name.c_str()) + ">纬度超范围");
                return false;
            }
            // 添加该位置
            route.pushSite(site);
        }
        if (urgentOut) {
            break;
        }

        // 哈希表存储
        this->routesHashMap.insert(make_pair(route.getName(), (int)this->routes.size()));
        // 数据存储
        this->routes.push_back(route);
        addRoutesName.push_back(route.getName());

        // 判断是否结束
        sstream >> temp;
        if (temp == continueSign) {
            continue;  // 开始循环
        }
        else if (temp == endSign) {
            break;     // 结束循环
        }
    }

    // 发送更改信号
    emit this->back(ROUTECHANGE);

    // 画出添加路线信息
    for (int i = 0; i < addRoutesName.size(); i++) {
        this->drawRoute(addRoutesName[i]);
    }

    // 清除历史记录 并 更新列表
    this->ui->textEditAddRouteText->clear();
    this->ui->comboBoxDelete->clear();
    for (int i = 0; i < this->routes.size(); i++) {
        this->ui->comboBoxDelete->addItem(QString::fromLocal8Bit(this->routes[i].getName().c_str()));
    }

    // 给出提示
    QMessageBox::information(this, "SUCCESS", "添加成功");

    return true;
}
// @name   : deleteRouteDeal
// @time   : 2022.9.4
// @func   : 删除路线
bool routePage::deleteRouteDeal(void)
{
    string routeName = (string)(this->ui->comboBoxDelete->currentText().toLocal8Bit());
    // 路线选择
    if (routeName.size() == 0) {
        QMessageBox::warning(this, "提示", "请选择需要删除的路线");
        return false;
    }

    // 二次判断
    if (QMessageBox::No == QMessageBox::question(this, "注意", "确定删除<" + QString::fromLocal8Bit(routeName.c_str()) + ">?", QMessageBox::Yes | QMessageBox::No)) {
        return false;
    }

    // 开始删除操作
    for (auto it = this->routes.begin(); it != this->routes.end(); it++){
        if ((*it).getName() == routeName) {
            this->routes.erase(it);
            break;
        }
    }

    // 发送更改信号
    emit this->back(ROUTECHANGE);

    // 清除图像区域
    emit this->clearGraph();

    // 更新列表
    this->ui->comboBoxDelete->clear();
    for (int i = 0; i < this->routes.size(); i++) {
        this->ui->comboBoxDelete->addItem(QString::fromLocal8Bit(this->routes[i].getName().c_str()));
    }

    // 给出提示
    QMessageBox::information(this, "SUCCESS", "删除成功");

    return true;
}


/****************************************** 画图函数 ******************************************/
// @name   : drawStation
// @time   : 2022.9.2
// @func   : 画出对应的站点
#define NODERADIUS 4
#define DEFAULTCOLOR 0x000000
bool routePage::drawStation(string name)
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
bool routePage::drawEdge(string stationA, string stationB, QColor color)
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
double routePage::calcuateDis(string stationA, string stationB)
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
void routePage::drawRoute(string name)
{

    //// 界面清除
    //emit this->clearGraph();
    
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