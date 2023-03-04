//@time   : 2022.8.29
//@author : gonzalez
//@func   : 主页面实现函数

#include "MetroSH.h"
#include "gonQueue.h"
#include <fstream>
#include <QGraphicsEllipseItem>
#include <qlineedit.h>
#include <qmessagebox.h>
#include <qtimer.h>
#include <qdatetime.h>

// @name   : MetroSH
// @time   : 2022.9.2
// @func   : 构造函数
MetroSH::MetroSH(QWidget *parent): QMainWindow(parent)
{
    // 初始化
    this->ui = new Ui::MetroSHClass;
    this->ui->setupUi(this);
    this->internal.getRoutesFromFile();  // 从txt获取数据
    this->internal.routes2sites();       // 提取sites邻接矩阵
    this->internal.routes2lines();       // 提取lines数据
    this->sceneInitiate();               // scene场景坐标获取信息
    this->pageST = new stationPage;      // 初始化站点页面
    this->pageRO = new routePage;        // 初始化路线页面
    this->currentTimeLabel = new QLabel; // 存储时间Label

     // scence信息初始化
    this->scene = new QGraphicsScene;
    this->scene->setSceneRect(this->sceneRect);
    this->ui->graphicsView->setRenderHint(QPainter::Antialiasing);     // 精致绘图
    this->ui->graphicsView->setScene(this->scene);                     // 添加绘图场景
    this->ui->graphicsView->setDragMode(QGraphicsView::ScrollHandDrag);// 设置鼠标拖拽格式

    // comboBox初始化
    for (int i = 0; i < this->internal.sites.size(); i++) {
        this->ui->startBox->addItem(QString::fromLocal8Bit(this->internal.sites[i].name.c_str()));
        this->ui->endBox->addItem(QString::fromLocal8Bit(this->internal.sites[i].name.c_str()));
    }
    QLineEdit* line1 = new QLineEdit;
    line1->setPlaceholderText("请选择起始站");
    this->ui->startBox->setLineEdit(line1);
    QLineEdit* line2 = new QLineEdit;
    line2->setPlaceholderText("请选择终点站");
    this->ui->endBox->setLineEdit(line2);

    // 画出全局图像
    this->drawGraph();                                                 // 画出地铁全局图像


    /**************************************************  槽链接操作 **************************************************/

    // searchButton链接
    connect(this->ui->searchButton, &QPushButton::clicked, this, &MetroSH::searchProcedure);
    void(MetroSH:: * clearSign)(void) = &MetroSH::clearGraph;
    connect(this, clearSign, this->scene, &QGraphicsScene::clear);
    // 工具栏链接
    connect(this->ui->actionToolBar, &QAction::triggered, [=]() {
        if (this->ui->mainToolBar->isVisible()) {
            this->ui->mainToolBar->setVisible(false);
        }
        else {
            this->ui->mainToolBar->setVisible(true);
        }
        });
    // 换乘指南链接
    connect(this->ui->actionChangeGuide, &QAction::triggered, [=]() {
        if (this->ui->changeGuideArea->isVisible()) {
            this->ui->changeGuideArea->setVisible(false);
        }
        else {
            this->ui->changeGuideArea->setVisible(true);
        }
        });
    // 缩小链接
    connect(this->ui->actionShrink, &QAction::triggered, this->ui->graphicsView, &myQGraphicsView::shrink);
    // 放大链接
    connect(this->ui->actionExpand, &QAction::triggered, this->ui->graphicsView, &myQGraphicsView::expand);
    // 显示全局链接
    connect(this->ui->actionGraphShow, &QAction::triggered, this, &MetroSH::drawGraph);
    // 添加站点链接
    connect(this->ui->actionAddStation, &QAction::triggered, [=]() {

        this->transmit(STATIONCHANGE);   // 数据传送
        this->pageST->initiate();        // 根据传入参数初始化
        this->pageST->show();            // 显示站点修改页面 

        });
    // 添加路线链接
    connect(this->ui->actionAddRoute, &QAction::triggered, [=]() {
        this->transmit(ROUTESCHANGE);    // 数据传送
        this->pageRO->initiate();        // 根据传入参数初始化
        this->pageRO->show();            // 显示站点修改页面 
        });

    // 数据反馈
    void(stationPage:: * backSignST)(int) = &stationPage::back;
    void(routePage:: * backSighRO)(int) = &routePage::back;
    void(MetroSH:: * receiveSign)(int) = &MetroSH::receive;
    connect(this->pageST, backSignST, this, receiveSign);
    connect(this->pageRO, backSighRO, this, receiveSign);

    // 重置按钮
    connect(this->ui->actionRestart, &QAction::triggered, this, &MetroSH::restart);
    // 关闭窗口
    connect(this->ui->actionExit, &QAction::triggered, this, &QMainWindow::close);
    // 帮助按钮
    connect(this->ui->actionHelp, &QAction::triggered, [=]() {
        QString helpInfo = "🙂这里有一些系统基本介绍:\n";
        helpInfo += "    ☀ 文件(F)\n";
        helpInfo += "       重置☞抛弃现有修改，使用scr/data/subwayInfo_standard.txt作为标准，进行系统数据重构\n";
        helpInfo += "       退出☞退出地铁指南系统\n";
        helpInfo += "    ☀ 工具(T)\n";
        helpInfo += "       放大☞对页面进行放大(也可使用鼠标滚轮)\n";
        helpInfo += "       缩小☞对页面进行缩小(也可使用鼠标滚轮)\n";
        helpInfo += "       站点管理☞弹出站点管理对话框\n";
        helpInfo += "       路线管理☞弹出路线管理对话框\n";
        helpInfo += "       展示全线☞显示全部系统线路\n";
        helpInfo += "    ☀ 视图(V)\n";
        helpInfo += "       工具栏☞控制<工具栏>的显示与消失\n";
        helpInfo += "       换乘指南☞控制<换乘指南>的显示与消失\n";
        helpInfo += "    ☀ 帮助(H)\n";
        helpInfo += "       基本介绍☞就是此页内容🌙\n";
        helpInfo += "    ☀ 关于(A)\n";
        helpInfo += "       关于项目☞此项目的简单介绍\n";
        helpInfo += "       关于作者☞作者介绍及联系方式👨\n";
        QMessageBox::information(this, "基本介绍", helpInfo);
        });
    // 关于项目
    connect(this->ui->actionAboutProject, &QAction::triggered, [=]() {
        QString aboutProject = "😀这里有一些项目简介:\n";
        //aboutProject += "     时间⏱️: 2022.8.24 - 2022.9.4\n";
        //aboutProject += "     起因👻: 数据结构课设\n";
        //aboutProject += "     技术🤖: qt + C++\n";
        //aboutProject += "     工具🛠️: qtcreater/designer + vs2022\n";
        aboutProject += "     时间: 2022.8.24 - 2022.9.4\n";
        aboutProject += "     起因: 数据结构课设\n";
        aboutProject += "     技术: qt + C++\n";
        aboutProject += "     工具: qtcreater/designer + vs2022\n";
        QMessageBox::information(this, "关于项目", aboutProject);
        });
    // 关于作者
    connect(this->ui->actionAboutAuthor, &QAction::triggered, [=]() {
        QString aboutAuthor = "name  gonzalez👨\n";
        //aboutAuthor += "info   : 😎🦓👨‍💻🏂🏄‍♂️🍉🛹🌏\n";
        aboutAuthor += "info    ⚽/📷/🌏/🛹/👨‍💻...\n";
        aboutAuthor += "email  gonzalez_zhefei@163.com\n";
        aboutAuthor += "github https://github.com/ZhefeiGong\n";
        QMessageBox::information(this, "关于作者", aboutAuthor);
        });
    // 状态栏
    this->timer = new QTimer(this);
    this->timer->start(1000); 
    connect(this->timer, &QTimer::timeout, this, &MetroSH::timeUpdate);
    QLabel* author = new QLabel("Copyright 2022 gonzalez");
    this->ui->statusBar->addWidget(author);

}
// @name   : restart
// @time   : 2022.9.2
// @func   : 重置数据
void MetroSH::restart(void)
{
    if (QMessageBox::Yes == QMessageBox::question(this, "注意", "此举将清空现有系统数据，按照标准文件重置\n您确定这样做吗？", QMessageBox::Yes | QMessageBox::No)) {
        // 清空数据
        this->internal.routes.clear();           // 清空原始数据
        this->internal.clear();                  // 清空原始数据

        // 数据初始化
        this->internal.getRoutesFromFile(true);  // 从标准txt获取数据
        this->internal.routes2sites();           // 提取sites邻接矩阵
        this->internal.routes2lines();           // 提取lines数据
        this->sceneInitiate();                   // scene场景坐标获取信息

        // scence信息初始化
        this->scene = new QGraphicsScene;
        this->scene->setSceneRect(this->sceneRect);
        this->ui->graphicsView->setRenderHint(QPainter::Antialiasing);     // 精致绘图
        this->ui->graphicsView->setScene(this->scene);                     // 添加绘图场景
        this->ui->graphicsView->setDragMode(QGraphicsView::ScrollHandDrag);// 设置鼠标拖拽格式

        // comboBox初始化
        for (int i = 0; i < this->internal.sites.size(); i++) {
            this->ui->startBox->addItem(QString::fromLocal8Bit(this->internal.sites[i].name.c_str()));
            this->ui->endBox->addItem(QString::fromLocal8Bit(this->internal.sites[i].name.c_str()));
        }
        QLineEdit* line1 = new QLineEdit;
        line1->setPlaceholderText("请选择起始站");
        this->ui->startBox->setLineEdit(line1);
        QLineEdit* line2 = new QLineEdit;
        line2->setPlaceholderText("请选择终点站");
        this->ui->endBox->setLineEdit(line2);

        // 画出全局图像
        this->drawGraph();                                                 // 画出地铁全局图像
    }
    return;
}
// @name   : MetroSH
// @time   : 2022.9.2
// @func   : 析构函数
MetroSH::~MetroSH()
{
}
// @name   : closeEvent
// @time   : 2022.9.4
// @func   : 重写closeEvent函数
void MetroSH::closeEvent(QCloseEvent* event)
{   
    //-------- 系统数据保存 --------
    string path = "src/data/subwayInfo.txt";
    ofstream file_writer(path, ios_base::out);
    string name_ = "name";
    string numSite_ = "numSite";
    string colour_ = "colour";
    string fromTo_ = "fromTo";
    string continueSign = "----------";
    string endSign = "==========";
    for (int i = 0; i < this->internal.routes.size(); i++) {
        QColor color = this->internal.routes[i].getColor();
        QString redStr = QString("%1").arg(color.red(), 2, 16, QChar('0'));
        //g的rgb转化为16进制
        QString greenStr = QString("%1").arg(color.green(), 2, 16, QChar('0'));
        //b的rgb转化为16进制
        QString blueStr = QString("%1").arg(color.blue(), 2, 16, QChar('0'));
        //将各rgb的拼接在一起形成#000000
        string hexStr = (string)(("0x" + redStr + greenStr + blueStr).toLocal8Bit());

        file_writer << name_ <<" " << this->internal.routes[i].getName() << endl;
        file_writer << numSite_ << " " << this->internal.routes[i].getNum() << endl;
        file_writer << colour_ << " " << hexStr << endl;
        file_writer << fromTo_ << " " << this->internal.routes[i].getOrigin() << " " << this->internal.routes[i].getDestination() << endl;

        for (int j = 0; j < this->internal.routes[i].getNum(); j++) {
            file_writer << this->internal.routes[i].getSite(j).name << " ";
            file_writer << this->internal.routes[i].getSite(j).latitude << " ";
            file_writer << this->internal.routes[i].getSite(j).longitude << endl;
        }
        if (i != this->internal.routes.size() - 1) {
            file_writer << continueSign << endl;
        }
        else {
            file_writer << endSign << endl;
        }
    }
    file_writer.close();//关闭文件

    //-------- 其他页面关闭 --------
    this->pageST->close();
    this->pageRO->close();

    return;
}
// @name   : timeUpdate
// @time   : 2022.9.4
// @func   : 时间更新函数
void MetroSH::timeUpdate(void)
{
    QDateTime CurrentTime = QDateTime::currentDateTime();
    QString Timestr = CurrentTime.toString(" yyyy年-MM月-dd日 hh:mm:ss "); //设置显示的格式
    this->currentTimeLabel->setText(Timestr);
    this->ui->statusBar->addPermanentWidget(currentTimeLabel);
}
// @name   : pageST_transmit
// @time   : 2022.9.2
// @func   : 页面间数据传送
void MetroSH::transmit(int method)
{
    // 站点改变
    if (method == STATIONCHANGE) {
        this->pageST->routes = this->internal.routes;                 // 路线信息
        this->pageST->sites = this->internal.sites;                   // 站点信息的邻接列表
        this->pageST->lines = this->internal.lines;                   // 路线信息的邻接列表
        this->pageST->routesHashMap = this->internal.routesHashMap;   // 路线名字与index的哈希表
        this->pageST->sitesHashMap = this->internal.sitesHashMap;     // 站点名字与index的哈希表
        this->pageST->linesHashMap = this->internal.linesHashMap;     // 路线名字与index的哈希表

        this->pageST->sceneRect = this->sceneRect;                    // 画面长宽
        this->pageST->coordHashMap = this->coordHashMap;              // 显示坐标哈希表
    }
    // 路线改变
    else if(method == ROUTESCHANGE) {
        this->pageRO->routes = this->internal.routes;                 // 路线信息
        this->pageRO->sites = this->internal.sites;                   // 站点信息的邻接列表
        this->pageRO->lines = this->internal.lines;                   // 路线信息的邻接列表
        this->pageRO->routesHashMap = this->internal.routesHashMap;   // 路线名字与index的哈希表
        this->pageRO->sitesHashMap = this->internal.sitesHashMap;     // 站点名字与index的哈希表
        this->pageRO->linesHashMap = this->internal.linesHashMap;     // 路线名字与index的哈希表

        this->pageRO->sceneRect = this->sceneRect;                    // 画面长宽
        this->pageRO->coordHashMap = this->coordHashMap;              // 显示坐标哈希表
    }
    return;
}
// @name   : pageST_receive
// @time   : 2022.9.2
// @func   : 接受数据
void MetroSH::receive(int method)
{
    // 数据清理
    this->internal.clear();              // 数据清洗
    this->coordHashMap.clear();          // 站点坐标哈希表

    // 数据赋值
    if (method == STATIONCHANGE) {       // 站点改变
        this->internal.routes = this->pageST->routes;
    }
    else if (method == ROUTESCHANGE) {   // 路线改变
        this->internal.routes = this->pageRO->routes;
    }
   
    // 数据重构
    this->internal.routes2routesHash();  // 从新数据提取routes哈希表
    this->internal.routes2sites();       // 提取sites邻接矩阵
    this->internal.routes2lines();       // 提取lines数据
    this->sceneInitiate();               // scene场景坐标获取信息

    // 页面重新初始化
    this->scene->setSceneRect(this->sceneRect); 

    // comboBox初始化
    this->ui->startBox->clear();
    this->ui->endBox->clear();
    for (int i = 0; i < this->internal.sites.size(); i++) {
        this->ui->startBox->addItem(QString::fromLocal8Bit(this->internal.sites[i].name.c_str()));
        this->ui->endBox->addItem(QString::fromLocal8Bit(this->internal.sites[i].name.c_str()));
    }
    QLineEdit* line1 = new QLineEdit;
    line1->setPlaceholderText("请选择起始站");
    this->ui->startBox->setLineEdit(line1);
    QLineEdit* line2 = new QLineEdit;
    line2->setPlaceholderText("请选择终点站");
    this->ui->endBox->setLineEdit(line2);

    // 更新全局图像
    this->drawGraph();

    // 更新后数据传入
    this->transmit(method);

    return;
}
// @name   : searchProcedure
// @time   : 2022.9.2
// @func   : 寻找路线函数
void MetroSH::searchProcedure(void)
{
    string startName = (string)(this->ui->startBox->currentText().toLocal8Bit());
    string endName = (string)(this->ui->endBox->currentText().toLocal8Bit());

    if (startName.size() == 0) {
        QMessageBox::warning(this, "警告", "请输入起始站");
    }
    else if (endName.size() == 0) {
        QMessageBox::warning(this, "警告", "请输入终点站");
    }
    else {
        if (this->internal.sitesHashMap.find(startName) == this->internal.sitesHashMap.end()) {
            QMessageBox::warning(this, "警告", "<" + QString::fromLocal8Bit(startName.c_str()) + ">不存在");
        }
        else if (this->internal.sitesHashMap.find(endName) == this->internal.sitesHashMap.end()) {
            QMessageBox::warning(this, "警告", "<" + QString::fromLocal8Bit(endName.c_str()) + ">不存在");
        }
        else if(startName==endName){
            QMessageBox::warning(this, "警告", "终点和起点一致\n请重新输入");
        }
        else {
            // 判断是否可抵达
            bool arriviable = false;
            int startIndex = this->internal.sitesHashMap.find(startName)->second;
            int endIndex = this->internal.sitesHashMap.find(endName)->second;
            for (int i = 0; i < this->internal.sites[startIndex].viaLines.size(); i++) {
                for (int j = 0; j < this->internal.sites[endIndex].viaLines.size(); j++) {
                    if (this->isArrivible(this->internal.sites[startIndex].viaLines[i], this->internal.sites[endIndex].viaLines[j])) {
                        arriviable = true;
                    }
                }
            }
            if (!arriviable) {
                QMessageBox::warning(this, "警告", "<"+ QString::fromLocal8Bit(startName.c_str())+">无法抵达<"+ QString::fromLocal8Bit(endName.c_str())+">\n请考虑其他交通方式!!!");
                return;
            }
            // 进行搜索
            vector<string>ans;
            this->internal.clearAns();                               // 清除之前查找结果数据
            if (this->ui->changesMinButton->isChecked()) {           // 换乘最少
                this->internal.changesMinSerach(startName, endName);
                ans = this->internal.changesMinAns;
            }
            else {                                                   // 时间最短
                this->internal.timeMinSerach(startName, endName);
                ans = this->internal.timeMinAns;
            }
            this->drawAns(ans);                                      // 绘制相关图像
        }
    }

    return;
}
// @name   : isArrivible
// @time   : 2022.9.2
// @func   : 判断两条线路是否可抵达
bool MetroSH::isArrivible(string startLine, string endLine)
{
    // 特殊情况军可抵达
    if (startLine == endLine) {
        return true;
    }

    // 记录路线信息
    vector<vector<string>> bfsStore;

    // 数据初始化
    string tail = "NULL";
    gonQueue<string> queue;
    queue.push(startLine);
    queue.push(tail);
    vector<string> temp1 = { startLine };
    bfsStore.push_back(temp1);
    vector<string> temp2;
    bfsStore.push_back(temp2);

    // 广度优先搜索
    int layer = 1;
    bool breakSign = false;
    while (!(queue.size()==1&& queue.front()==tail)) {
        string top = queue.front();
        queue.pop();
        if (top == tail) {
            layer++;
            queue.push(tail);
            vector<string> temp;
            bfsStore.push_back(temp);
            continue;
        }
        else {
            int index = this->internal.linesHashMap.find(top)->second;
            for (int i = 0; i < this->internal.lines[index].lines.size(); i++) {
                bool find = false;
                for (int j = 0; j <= layer; j++) {
                    if (count(bfsStore[j].begin(), bfsStore[j].end(), this->internal.lines[index].lines[i]) != 0) {
                        find = true;
                        break;
                    }
                }
                if (!find) {
                    bfsStore[layer].push_back(this->internal.lines[index].lines[i]);
                    queue.push(this->internal.lines[index].lines[i]);
                    if (this->internal.lines[index].lines[i] == endLine) {
                        breakSign = true;
                        break;
                    }
                }
            }
        }
        if (breakSign)
            break;
    }
    
    return breakSign;
}
// @name   : sceneInitiate
// @time   : 2022.9.2
// @func   : 根据经纬度信息更新坐标情况
// @notice : 
// 0 — X
// |
// Y
#define SCENEEXPANDTIMES 3 
void MetroSH::sceneInitiate(void)
{
    vector<int> xList;
    vector<int> yList;
    for (int i = 0; i < this->internal.sites.size(); i++) {
        int tempX = (int)(this->internal.sites[i].longitude * EXPANDTIMES);
        int tempY = (int)(this->internal.sites[i].latitude * EXPANDTIMES);
        xList.push_back(tempX);
        yList.push_back(tempY);
    }
    int minX = *min_element(xList.begin(), xList.end()) - SCENESPACE;
    int maxX = *max_element(xList.begin(), xList.end()) + SCENESPACE;
    int minY = *min_element(yList.begin(), yList.end()) - SCENESPACE;
    int maxY = *max_element(yList.begin(), yList.end()) + SCENESPACE;
    
    // 设置scene场景大小
    this->sceneRect.setTopLeft(QPoint(0, 0));
    this->sceneRect.setBottomRight(QPoint((maxX - minX)* SCENEEXPANDTIMES, (maxY - minY)* SCENEEXPANDTIMES));
    
    // 设置站点图像坐标
    for (int i = 0; i < this->internal.sites.size(); i++) {
        int x = ((int)(this->internal.sites[i].longitude * EXPANDTIMES) - minX)* SCENEEXPANDTIMES;
        int y = ((maxY - minY) * SCENEEXPANDTIMES) - ((int)(this->internal.sites[i].latitude * EXPANDTIMES) - minY) * SCENEEXPANDTIMES;
        this->coordHashMap.insert(make_pair(this->internal.sites[i].name, QPoint(x, y)));
    }
    return;
}


/****************************************** 画图函数 ******************************************/
// @name   : drawAns
// @time   : 2022.9.3
// @func   : 绘制结果
bool MetroSH::drawAns(vector<string> ans)
{
    // 清除原有数据
    emit this->clearGraph();
    this->ui->graphicsView->refresh();
    this->ui->serchTextArea->clear();

    // 找到路线
    vector<vector<string>> lineAns;
    for (int i = 0; i < ans.size() - 1; i++) {
        siteInfo site1 = this->internal.sites[this->internal.sitesHashMap.find(ans[i])->second];
        siteInfo site2 = this->internal.sites[this->internal.sitesHashMap.find(ans[i + 1])->second];
        vector<string> temp;

        for (int j = 0; j < site1.viaLines.size(); j++) {
            for (int k = 0; k < site2.viaLines.size(); k++) {
                if (site1.viaLines[j] == site2.viaLines[k]) {
                    temp.push_back(site1.viaLines[j]);
                }
            }
        }
        lineAns.push_back(temp);
    }
    // 找到换乘站
    vector<string> changeSites;
    for (int i = 1; i < (ans.size() - 1); i++) {
        bool change = true;
        for (int j = 0; j < lineAns[i - 1].size(); j++) {
            for (int k = 0; k < lineAns[i].size(); k++) {
                if (lineAns[i - 1][j] == lineAns[i][k]) {
                    change = false;
                    break;
                }
            }
        }
        if (change) {
            changeSites.push_back(ans[i]);
        }
    }
    // searchText显示结果
    ui->serchTextArea->setText("共经过 " + QString::number(ans.size()) + " 站");
    ui->serchTextArea->append("起点站 " + QString::fromLocal8Bit(ans[0].c_str()));
    for (int i = 1; i < ans.size(); i++) {
        QString temp;
        if (count(changeSites.begin(), changeSites.end(), ans[i - 1]) != 0) {
            temp += "       换乘";
        }
        else {
            temp += "              ";
        }
        temp += " ↓ 乘坐";
        for (int j = 0; j < lineAns[i - 1].size(); j++) {
            temp += QString::fromLocal8Bit(lineAns[i - 1][j].c_str()) + " ";
        }
        ui->serchTextArea->append(temp);

        if (i != ans.size() - 1) {
            ui->serchTextArea->append("           " + QString::fromLocal8Bit(ans[i].c_str()));
        }
        else {
            ui->serchTextArea->append("终点站 " + QString::fromLocal8Bit(ans[i].c_str()));
        }
    }

    // 绘制结果图
    for (int i = 0; i < (ans.size() - 1); i++) {
        siteInfo site1 = this->internal.sites[this->internal.sitesHashMap.find(ans[i])->second];
        siteInfo site2 = this->internal.sites[this->internal.sitesHashMap.find(ans[i + 1])->second];
        for (int j = 0; j < site1.viaLines.size(); j++) {
            for (int k = 0; k < site2.viaLines.size(); k++) {
                if (site1.viaLines[j] == site2.viaLines[k]) {
                    QColor color = this->internal.routes[this->internal.routesHashMap.find(site1.viaLines[j])->second].getColor();
                    this->drawEdge(site1.name, site2.name, color);
                }
            }
        }
    }
    for (int i = 0; i < ans.size(); i++) {
        this->drawStation(ans[i]);
    }

    return true;
}
// @name   : calcuateDis
// @time   : 2022.9.2
// @func   : 计算两个站点之间距离
double MetroSH::calcuateDis(string stationA, string stationB)
{
    double longi_A = this->internal.sites[this->internal.sitesHashMap.find(stationA)->second].longitude;
    double lati_A = this->internal.sites[this->internal.sitesHashMap.find(stationA)->second].latitude;
    double longi_B = this->internal.sites[this->internal.sitesHashMap.find(stationB)->second].longitude;
    double lati_B = this->internal.sites[this->internal.sitesHashMap.find(stationB)->second].latitude;

    double distance = sqrt(pow((longi_A - longi_B), 2) + pow((lati_A - lati_B), 2)) * 111;
    return distance;
}
// @name   : drawGraph
// @time   : 2022.9.2
// @func   : 画出总体图像
bool MetroSH::drawGraph(void)
{
    // 清除原有数据
    emit this->clearGraph();
    this->ui->graphicsView->refresh();
    this->ui->serchTextArea->clear();
    this->ui->startBox->clearEditText();
    this->ui->endBox->clearEditText();
    this->ui->changesMinButton->setChecked(true);
    this->ui->timeMinButton->setChecked(false);

    // 画出对应的连线
    for (int i = 0; i < this->internal.routes.size(); i++) {
        for (int j = 0; j < (this->internal.routes[i].getNum() - 1); j++) {
            string nameA = this->internal.routes[i].getSite(j).name;
            string nameB = this->internal.routes[i].getSite(j + 1).name;
            this->drawEdge(nameA, nameB, this->internal.routes[i].getColor());
        }
    }
    // 画出点
    for (int i = 0; i < this->internal.sites.size(); i++) {
        this->drawStation(this->internal.sites[i].name);
    }

    return true;
}
// @name   : drawStation
// @time   : 2022.9.2
// @func   : 画出对应的站点
#define NODERADIUS 3
#define DEFAULTCOLOR 0x000000
bool MetroSH::drawStation(string name)
{
    QPoint coord = this->coordHashMap.find(name)->second;                                 // 坐标信息
    siteInfo site = this->internal.sites[this->internal.sitesHashMap.find(name)->second]; // 站点信息

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
        int index = this->internal.routesHashMap.find(site.viaLines[0])->second;
        siteColor = this->internal.routes[index].getColor();
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
// @time   : 2022.9.2
// @func   : 链接对应的站点
#define LINE_WIDTH  1
bool MetroSH::drawEdge(string stationA, string stationB, QColor color)
{
    QPoint coordA = this->coordHashMap.find(stationA)->second;
    QPoint coordB = this->coordHashMap.find(stationB)->second;
    siteInfo siteA = this->internal.sites[this->internal.sitesHashMap.find(stationA)->second]; // 站点信息
    siteInfo siteB = this->internal.sites[this->internal.sitesHashMap.find(stationB)->second]; // 站点信息

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
        int index = this->internal.routesHashMap.find(commonLine[i])->second;
        QString origin = QString::fromLocal8Bit(this->internal.routes[index].getOrigin().c_str());
        QString destination = QString::fromLocal8Bit(this->internal.routes[index].getDestination().c_str());
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