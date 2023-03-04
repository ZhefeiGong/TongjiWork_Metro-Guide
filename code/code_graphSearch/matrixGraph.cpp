//@timae  : 2022.8.25 - 2022.8.29
//@author : gonzalez
//@func   : 可视化执行文件

#include "matrixGraph.h"
#include <vector>
#include <qdebug.h>
#include <qbytearray.h>
#include <qpushbutton.h>
#include <string.h>
#include <qmessagebox.h>
#include <math.h>
#include <qpixmap.h>
#include <qpainter.h>
#include <cstdlib>
#include <time.h>

using namespace std;

/************************************************ matrixGraph ************************************************/
//@name : matrixGraph
//@func : 构造函数
//@time : 2022.8.27
matrixGraph::matrixGraph(QWidget *parent): QMainWindow(parent)
{
	// initiate初始化
	this->ui = new Ui::matrixGraphClass;
    this->ui->setupUi(this);
	this->inputPage = new inputWindow;            // 实例化输入界面
	this->algorithm = new matrixGraphAlgorithm;   // 初始化算法数据类
	this->inputMethod = NONINPUT;                 // 初始化输入方式
	this->vertexSite = NULL;                      // 图顶点坐标
	this->timerDfs = new QTimer;                  // Dfs定时器类
	this->timerBfs = new QTimer;                  // Bfs定时器类
	this->func = -1;                              // 无功能显示

	this->recordDfs = "";                         // 记录栈的情况方便图形化显示
	this->numDfs = 0;
	this->recordBfs = "";                         // 记录队列的情况方便图形化显示
	this->numBfs = 0;

	// 输入inputAllBtn
	connect(ui->inputAllBtn, &QPushButton::clicked, [=]() {

		if (this->timerDfs->isActive() || this->timerBfs->isActive()) {
			QMessageBox::information(this, "notice", "请等待搜索过程结果");
		}
		else {
			this->ui->ansBox->clear();//清除结果显示
			this->hide();             //主窗口隐藏
			this->inputPage->show();  //次窗口显示

			// 提示输入
			QString showStr = "请直接输入图的邻接矩阵形式(参照以下形式)\n 0 0 1 1\n 0 1 0 0\n 1 0 0 0 \n 1 0 0 0";
			this->inputPage->noticeUpdate(showStr);

			this->inputMethod = ALLINPUT;  // 输入标志
		}

		});

	// 输入inputEachBtn
	connect(ui->inputEachBtn, &QPushButton::clicked, [=]() {

		if (this->timerDfs->isActive() || this->timerBfs->isActive()) {
			QMessageBox::information(this, "notice", "请等待搜索过程结果");
		}
		else {
			this->ui->ansBox->clear();//清除结果显示
			this->hide();             //主窗口隐藏
			this->inputPage->show();  //次窗口显示

			// 提示输入
			QString showStr = "请输入图连接形式(参照以下形式)\n 4  (顶点数) \n 0 1  (顶点0和顶点1相连)\n 2 3  (顶点2和顶点3相连)\n ...";
			this->inputPage->noticeUpdate(showStr);

			this->inputMethod = EACHINPUT; // 输入标志
		}
		});

	// 次窗口返回信号
	void (inputWindow:: * backSignal)(QString) = &inputWindow::back;
	void (matrixGraph:: * dealSignal)(QString) = &matrixGraph::backDeal;
	connect(this->inputPage, backSignal, this, dealSignal);

	// LIST_show按钮
	connect(ui->listShowBtn, &QPushButton::clicked, [=]() {

		if (this->timerDfs->isActive() || this->timerBfs->isActive()) {
			QMessageBox::information(this, "notice", "请等待搜索过程结果");
		}
		else {
			this->ui->ansBox->clear();     // 清除结果显示
			if (this->inputMethod != NONINPUT) {
				// 功能显示更改
				this->func = LITS_SHOW;
				// 更新邻接列表
				this->drawList();
				QPixmap pix;
				pix.load("image/list.png");
				QPixmap fitpixmap = pix.scaled(280, 420, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
				this->ui->displayLabel->setPixmap(fitpixmap);
				// 更新邻接矩阵
				this->drawMatrix();
				QPixmap pixMatrix;
				pixMatrix.load("image/matrix.png");
				QPixmap fitpixmap_matrix = pixMatrix.scaled(280, 420, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
				this->ui->graphLabel->setPixmap(fitpixmap_matrix);
			}
			else {
				QMessageBox::information(this, "notice", "请先输入图信息");
			}
		}
		});

	// DFS_RE按钮
	connect(ui->dfsReBtn, &QPushButton::clicked, [=]() {
		if (this->timerDfs->isActive() || this->timerBfs->isActive()) {
			QMessageBox::information(this, "notice", "请等待搜索过程结果");
		}
		else {
			if (this->inputMethod != NONINPUT) {

				// 功能显示更改
				this->func = DFS_RECUR;

				// 开始显示
				this->algorithm->clearAns();       // 清除执行结果
				this->algorithm->dfsRecursion(0);  // 递归执行结果
				this->timerDfs->start(TIMEINTER);  // 开启计时器

				// 显示结果
				QFont font;
				font.setPointSize(12);
				this->ui->ansBox->setFont(font);

				QString ans = "递归深度优先搜索结果为:\n";
				for (int i = 0; i < this->algorithm->numVertex; i++) {
					QString temp = QString::number(this->algorithm->indexAnsDfs[i]);
					ans = ans + temp;
				}
				this->ui->ansBox->setText(ans);

			}
			else {
				QMessageBox::information(this, "notice", "请先输入图信息");
			}
		}
		});

	// DFS_NONRE按钮
	connect(ui->dfsNonreBtn, &QPushButton::clicked, [=]() {
		if (this->timerDfs->isActive() || this->timerBfs->isActive()) {
			QMessageBox::information(this, "notice", "请等待搜索过程结果");
		}
		else {
			if (this->inputMethod != NONINPUT) {

				// 功能显示更改
				this->func = DFS_NONRECUR;

				// 开始显示
				this->algorithm->clearAns();          // 清除执行结果
				this->algorithm->dfsNonRecursion(0);  // 递归执行结果
				this->timerDfs->start(TIMEINTER);     // 开启计时器

				// 显示结果
				QFont font;
				font.setPointSize(12);
				this->ui->ansBox->setFont(font);

				QString ans = "非递归深度优先搜索结果为:\n";
				for (int i = 0; i < this->algorithm->numVertex; i++) {
					QString temp = QString::number(this->algorithm->indexAnsDfs[i]);
					ans = ans + temp;
				}
				this->ui->ansBox->setText(ans);
			}
			else {
				QMessageBox::information(this, "notice", "请先输入图信息");
			}
		}
		});

	// BFS_NONRE按钮
	connect(ui->bfsNonrebtn, &QPushButton::clicked, [=]() {
		if (this->timerDfs->isActive() || this->timerBfs->isActive()) {
			QMessageBox::information(this, "notice", "请等待搜索过程结果");
		}
		else {
			if (this->inputMethod != NONINPUT) {

				// 功能显示更改
				this->func = BFS_NONRECUR;

				// 开始显示
				this->algorithm->clearAns();          // 清除执行结果
				this->algorithm->bfsNonRecursion(0);  // 非递归执行结果
				this->timerBfs->start(TIMEINTER);     // 开启计时器

				// 显示结果
				QFont font;
				font.setPointSize(12);
				this->ui->ansBox->setFont(font);

				QString ans = "非递归广度优先搜索结果为:\n";
				for (int i = 0; i < this->algorithm->numVertex; i++) {
					QString temp = QString::number(this->algorithm->indexAnsBfs[i]);
					ans = ans + temp;
				}
				this->ui->ansBox->setText(ans);
			}
			else {
				QMessageBox::information(this, "notice", "请先输入图信息");
			}
		}
		});

	// Dfs定时器类
	connect(this->timerDfs, &QTimer::timeout, [=]() {
		moveNode get = this->algorithm->moveDfs.top();
		// 显示栈结果
		bool sign = this->drawStack();
		QPixmap pixStack;
		pixStack.load("image/stack.png");
		QPixmap fitpixmap_Stack = pixStack.scaled(280, 420, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
		this->ui->displayLabel->setPixmap(fitpixmap_Stack);
		// 显示矩阵结果
		if (sign) {
			this->drawMatrix(this->algorithm->indexAnsDfs, this->numDfs);
			QPixmap pixMatrix;
			pixMatrix.load("image/matrix.png");
			QPixmap fitpixmap_matrix = pixMatrix.scaled(280, 420, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
			this->ui->graphLabel->setPixmap(fitpixmap_matrix);
		}
		// 结束
		if (get.nodeIndex == NODENULL) {
			this->timerDfs->stop();
			this->numDfs = 0;
			this->recordDfs = "";
		}
		});

	// Bfs定时器类
	connect(this->timerBfs, &QTimer::timeout, [=]() {
		moveNode get = this->algorithm->moveBfs.top();
		// 显示栈结果
		bool sign = this->drawQueue();
		QPixmap pixQueue;
		pixQueue.load("image/queue.png");
		QPixmap fitpixmap_Queue = pixQueue.scaled(280, 420, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
		this->ui->displayLabel->setPixmap(fitpixmap_Queue);
		// 显示矩阵结果
		if (sign) {
			this->drawMatrix(this->algorithm->indexAnsBfs, this->numBfs);
			QPixmap pixMatrix;
			pixMatrix.load("image/matrix.png");
			QPixmap fitpixmap_matrix = pixMatrix.scaled(280, 420, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
			this->ui->graphLabel->setPixmap(fitpixmap_matrix);
		}
		// 结束
		if (get.nodeIndex == NODENULL) {
			this->timerBfs->stop();
			this->numBfs = 0;
			this->recordBfs = "";
		}
		});

}
//@name : ~matrixGraph
//@func : 析构函数
//@time : 2022.8.27
matrixGraph::~matrixGraph()
{
    delete ui;
}
//@name : backDeal
//@func : 处理输入数据函数
//@time : 2022.8.27
void matrixGraph::backDeal(QString data)
{
	// 页面转化
	this->inputPage->hide();  //主窗口显示
	this->show();             //次窗口隐藏
	
	QByteArray ba = data.toLatin1();
	char* str = ba.data();

	// 输入错误
	if (!this->Str2Algorithm(str)) {
		QMessageBox::warning(this, "warning", "输入错误");
	}
	// 输入正确
	else {
		// 输入成功反馈
		QMessageBox::information(this, "notice", "输入正确");

		// test show
		cout << "matrix:" << endl;    // for test
		this->algorithm->showMatrix();// for test

		// 列表更新矩阵
		this->algorithm->matrixUpdataList();

		// 画出矩阵
		this->ui->displayLabel->clear();     // 清除display页面
		this->siteGenerate();                // 矩阵位置更新
		this->drawMatrix();                  // 生成第一张矩阵图
		QPixmap pix;
		pix.load("image/matrix.png");
		QPixmap fitpixmap = pix.scaled(280, 420, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
		this->ui->graphLabel->setPixmap(fitpixmap);
	}
}
//@name : Str2Algorithm
//@func : 处理输入数据函数
//@time : 2022.8.27
bool matrixGraph::Str2Algorithm(char* str)
{	
	// change '\n' to ' ' and to string
	for (int i = 0; i < strlen(str); i++) {
		if (str[i] == '\n') {
			str[i] = ' ';
		}
	}
	string strString = string(str);
	
	// 提取string中数字
	vector<string> nums;
	splitString(nums, strString, " ");
	
	// 依次读取信息
	if (this->inputMethod == ALLINPUT) {
		if (int(sqrt(nums.size())) * int(sqrt(nums.size())) != nums.size()) {
			return false;
		}
		// 开始初始化
		else {
			this->algorithm->clear();// 清除历史信息
			int numV = sqrt(nums.size());
			bool* matrix = new bool[nums.size()];
			for (int i = 0; i < (int)nums.size(); i++) {
				if (stoi(nums[i]) == 0) {
					matrix[i] = false;
				}
				else {
					matrix[i] = true;
				}
			}
			this->algorithm->initiate(numV);
			this->algorithm->buildMatrixAll(matrix);
			delete[] matrix;
		}
	}
	else if (this->inputMethod == EACHINPUT) {
		if (nums.size() % 2 != 1) {
			return false;
		}
		// 开始初始化
		else{
			this->algorithm->clear();// 清除历史信息
			int numV = stoi(nums[0]);
			this->algorithm->initiate(numV);
			for (int i = 1; i < nums.size(); i+=2) {
				int num1 = stoi(nums[i]);
				int num2 = stoi(nums[i + 1]);
				this->algorithm->buildMatrixEach(num1, num2);
			}
		}
	}
	return true;// 成功初始化
}
//@name : siteGenerate
//@func : 随机生成矩阵坐标
//@time : 2022.8.27
bool matrixGraph::siteGenerate(void)
{
	// 存储空间更新
	if (this->vertexSite != NULL) {
		delete[] this->vertexSite;
	}
	this->vertexSite = new QPoint[this->algorithm->numVertex];

	// 定义位置
	int expand = 50;
	int areaSizeX = this->algorithm->numVertex * expand;
	int areaSizeY = this->algorithm->numVertex * expand * 3 / 2;

	// 随机生成坐标
	srand((unsigned)time(NULL));
	for (int i = 0; i < this->algorithm->numVertex; i++) {
		// site1
		while (true) {
			int get = rand()%areaSizeX;
			if (get >= RADIUS && get <= areaSizeX - RADIUS) {
				this->vertexSite[i].setX(get);
				break;
			}
		}
		// site2
		while (true) {
			int get = rand() % areaSizeY;
			if (get >= (RADIUS + RADIUS / 2) && get <= areaSizeY - (RADIUS + RADIUS / 2)) {
				this->vertexSite[i].setY(get);
				break;
			}
		}
		// 距离别太近
		for (int j = 0; j < i; j++) {
			int disX = (this->vertexSite[j].x() - this->vertexSite[i].x());
			int disY = (this->vertexSite[j].y() - this->vertexSite[i].y());
			int distance = sqrt(disX * disX + disY * disY);
			if (distance <= int(RADIUS * (2.5))) {
				i--;
				break;
			}
		}
		// 尽量不共线
		for (int j = 0; j < i; j++) {
			double mid = 0.01;
			double before = (double)this->vertexSite[j].x() / (double)this->vertexSite[j].y();
			double now = (double)this->vertexSite[i].x() / (double)this->vertexSite[i].y();
			if (before >= now - mid && before <= now + mid) {
				i--;
				break;
			}
		}
	}
	return true;

}
//@name : drawMatrix
//@func : 矩阵图片绘制
//@time : 2022.8.27
bool matrixGraph::drawMatrix(int* chooseV,int numV)
{
	// 颜色初始化
	Qt::GlobalColor VERTEXNONCOLOR = Qt::darkBlue;
	Qt::BrushStyle VERTEXNONSTYLE = Qt::Dense7Pattern;
	Qt::GlobalColor VERTEXCHOOSEDCOLOR = Qt::darkRed;
	Qt::BrushStyle VERTEXCHOOSEDSTYLE = Qt::Dense3Pattern;

	// 参数初始化
	int expand = 50;
	int areaLenX = this->algorithm->numVertex * expand;
	int areaLenY = this->algorithm->numVertex * expand * 3 / 2;
	QPixmap pix(areaLenX, areaLenY);
	pix.fill(Qt::transparent);                               // 用透明色填充
	QPainter painter(&pix);
	painter.setRenderHint(QPainter::Antialiasing);           // 增加抗锯齿能力

	// 开始画图
	for (int i = 0; i < this->algorithm->numVertex; i ++) {

		// 画顶点
		Qt::GlobalColor mycolor = VERTEXNONCOLOR;
		Qt::BrushStyle mystyle = VERTEXNONSTYLE;
		bool color = false;
		for (int j = 0; j < numV; j++) {
			if (chooseV[j] == i) {
				mycolor = VERTEXCHOOSEDCOLOR;
				mystyle = VERTEXCHOOSEDSTYLE;
				color = true;
				break;
			}
		}
		QPen pen;
		QBrush brush;
		pen.setColor(mycolor);
		pen.setWidth(2);
		brush.setColor(mycolor);
		brush.setStyle(mystyle);
		painter.setPen(pen);
		painter.setBrush(brush);
		painter.drawEllipse(this->vertexSite[i], RADIUS, RADIUS);
		
		// 画点名
		painter.drawText(QPoint(this->vertexSite[i].x() - RADIUS / 2, this->vertexSite[i].y() + RADIUS*3/2), QString("%1").arg(i));

		// 画线
		for (int j = 0; j < this->algorithm->numVertex; j++) {
			// 仅考虑矩阵一半
			if (i <= j) {
				// 存在线
				if (this->algorithm->adjacentMatrix[i][j] == true) {
					if (color) {
						mycolor = VERTEXNONCOLOR;
						mystyle = VERTEXNONSTYLE;
						for (int k = 0; k < numV; k++) {
							if (chooseV[k] == j) {
								mycolor = VERTEXCHOOSEDCOLOR;
								mystyle = VERTEXCHOOSEDSTYLE;
								break;
							}
						}
					}
					pen.setColor(mycolor);
					brush.setColor(mycolor);
					painter.setPen(pen);
					painter.setBrush(brush);
					painter.drawLine(this->vertexSite[i], this->vertexSite[j]);
				}
			}
		}
	}

	pix.save("./image/matrix.png");

	return true;
}
//@name : drawList
//@func : 列表图片绘制
//@time : 2022.8.27
bool matrixGraph::drawList(void)
{
	// 确定areaY
	int areaY = this->algorithm->numVertex * 60;
	// 确定areaX
	int numMaxX = 0;
	for (int i = 0; i < this->algorithm->numVertex; i++) {
		int record = 0;
		adjacentListNodePointer temp = this->algorithm->adjacentList[i].next;
		while (temp != NULL) {
			temp = temp->next;
			record++;
		}
		if (record > numMaxX) {
			numMaxX = record;
		}
	}
	int areaX = (numMaxX + 1) * 60;

	// 初始化操作
	QPixmap pix(areaX, areaY);
	pix.fill(Qt::transparent);                               // 用透明色填充
	QPainter painter(&pix);                                  // 确定画图位置
	painter.setRenderHint(QPainter::Antialiasing);           // 增加抗锯齿能力
	QPen pen;
	pen.setColor(Qt::darkMagenta);
	pen.setWidth(2);
	painter.setPen(pen);

	int mid = 10;

	for (int i = 0; i < this->algorithm->numVertex; i++) {

		// 画出本体
		int i_midX = 10 + 10;
		int i_midY = 20 + 35 * i;

		painter.drawRect(QRect(QPoint(i_midX - mid, i_midY - mid), QPoint(i_midX + mid, i_midY + mid)));
		painter.drawText(QRect(QPoint(i_midX - mid / 2, i_midY - mid / 2), QPoint(i_midX + mid, i_midY + mid)), QString("%1").arg(i));

		// 画出子体
		int j = 0;
		adjacentListNodePointer temp = this->algorithm->adjacentList[i].next;
		while (temp != NULL) {
			j++;
			int j_midX = i_midX + j * 35;
			int j_midY = i_midY;
			painter.drawRect(QRect(QPoint(j_midX - mid, j_midY - mid), QPoint(j_midX + mid, j_midY + mid)));
			painter.drawText(QRect(QPoint(j_midX - mid / 2, j_midY - mid / 2), QPoint(j_midX + mid, j_midY + mid)), QString("%1").arg(temp->nodeIndex));
			painter.drawLine(QPoint(i_midX, i_midY), QPoint(j_midX, j_midY)); // 连线
			temp = temp->next;
		}
	}


	pix.save("./image/list.png");

	return true;
}
//@name : drawStack
//@func : 栈绘制
//@time : 2022.8.28
bool matrixGraph::drawStack(void)
{
	bool feedback = false;        // matrix更新情况

	// 参数初始化
	moveNode get = this->algorithm->moveDfs.pop();
	int element = NODENULL;
	if (get.movement == NODEIN) {
		if (this->func != DFS_NONRECUR) {
			this->numDfs++;           // 入栈则遍历
			feedback = true;          // 需要更新graph
		}
		element = get.nodeIndex;
		this->recordDfs = to_string(get.nodeIndex) + this->recordDfs;
	}
	else if (get.movement == NODEOUT) {
		if (this->func == DFS_NONRECUR) {
			this->numDfs++;           // 出栈则遍历
			feedback = true;          // 需要更新graph
		}
		element = (this->recordDfs[0]) - '0';
		this->recordDfs = this->recordDfs.substr(1, (this->recordDfs.size() - 1));
	}

	// 画笔初始化
	int areaX = 200;
	int areaY = this->algorithm->numVertex * 60;
	QPixmap pix(areaX, areaY);
	pix.fill(Qt::transparent);                               // 用透明色填充
	QPainter painter(&pix);                                  // 画笔描绘地点
	painter.setRenderHint(QPainter::Antialiasing);           // 增加抗锯齿能力
	QPen pen;
	pen.setWidth(2);
	painter.setPen(pen);
	QPoint startSiteLeft(50, 60);
	QPoint endSiteLeft(50, 80 + this->recordDfs.size() * 50 + 2);

	// 开始画图
	for (int i = 0; i < this->recordDfs.size(); i++) {
		int midX = 80;
		int midY = 80 + 50 * i;
		if (get.movement == NODEIN && i == 0) {
			pen.setColor(Qt::darkRed);
		}
		else {
			pen.setColor(Qt::darkYellow);
		}
		painter.setPen(pen);
		painter.drawEllipse(QPoint(midX, midY), 20, 20);
		painter.drawText(QPoint(midX, midY), QString("%1").arg(this->recordDfs[i]));
	}
	pen.setColor(Qt::darkGray);
	painter.setPen(pen);
	painter.drawLine(startSiteLeft, endSiteLeft);
	painter.drawLine(QPoint(startSiteLeft.x() + 60, startSiteLeft.y()), QPoint(endSiteLeft.x() + 60, endSiteLeft.y()));
	painter.drawLine(endSiteLeft, QPoint(endSiteLeft.x() + 60, endSiteLeft.y()));

	// 画出箭头指示
	if (element != NODENULL) {
		pen.setColor(Qt::darkGray);
		painter.setPen(pen);
		QPolygon triangle;
		painter.drawRect(QRect(QPoint(150, 40), QPoint(170, 120)));     // 画矩形
		if (get.movement == NODEOUT) {
			triangle.setPoints(3, 140, 40, 160, 20, 180, 40);           // 三点坐标
		}
		else if (get.movement == NODEIN) {
			triangle.setPoints(3, 140, 120, 160, 135, 180, 120);        // 三点坐标
		}
		painter.drawPolygon(triangle);                                  // 画三角形

		pen.setColor(Qt::red);
		painter.setPen(pen);

		painter.drawEllipse(QPoint(160, 145), 10, 10);
		painter.drawText(QPoint(155, 148), QString("%1").arg(element));
	}

	pix.save("./image/stack.png");

	return feedback;
}
//@name : drawQueue
//@func : 队列绘制
//@time : 2022.8.27
bool matrixGraph::drawQueue(void)
{

	bool feedback = false;              // matrix更新情况

	// 参数初始化
	moveNode get = this->algorithm->moveBfs.pop();
	int element = NODENULL;
	if (get.movement == NODEIN) {
		this->numBfs++;                 // 入队列则遍历
		feedback = true;                // 需要更新graph
		element = get.nodeIndex;
		this->recordBfs = this->recordBfs + to_string(get.nodeIndex);
	}
	else if (get.movement == NODEOUT) {
		element = (this->recordBfs[0]) - '0';
		this->recordBfs = this->recordBfs.substr(1, (this->recordBfs.size() - 1));
	}

	// 画笔初始化
	int areaX = 200;
	int areaY = this->algorithm->numVertex * 60;
	QPixmap pix(areaX, areaY);
	pix.fill(Qt::transparent);                               // 用透明色填充
	QPainter painter(&pix);                                  // 画笔描绘地点
	painter.setRenderHint(QPainter::Antialiasing);           // 增加抗锯齿能力
	QPen pen;
	pen.setWidth(2);
	painter.setPen(pen);
	QPoint startSiteLeft(50, 60);
	QPoint endSiteLeft(50, 80 + this->recordBfs.size() * 50 + 2);

	// 开始画图
	for (int i = 0; i < this->recordBfs.size(); i++) {
		int midX = 80;
		int midY = 80 + 50 * i;
		if (get.movement == NODEOUT) {
			pen.setColor(Qt::darkYellow);
		}
		else if (get.movement == NODEIN) {
			if (i == recordBfs.size() - 1) {
				pen.setColor(Qt::darkRed);
			}
			else {
				pen.setColor(Qt::darkYellow);
			}
		}
		painter.setPen(pen);
		painter.drawEllipse(QPoint(midX, midY), 20, 20);
		painter.drawText(QPoint(midX, midY), QString("%1").arg(this->recordBfs[i]));
	}
	pen.setColor(Qt::darkGray);
	painter.setPen(pen);
	painter.drawLine(startSiteLeft, endSiteLeft);
	painter.drawLine(QPoint(startSiteLeft.x() + 60, startSiteLeft.y()), QPoint(endSiteLeft.x() + 60, endSiteLeft.y()));
	painter.drawText(QPoint(20, 60), QString("head"));
	painter.drawText(QPoint(20, 80 + this->recordBfs.size() * 50 + 2), QString("tail"));

	// 画出箭头指示
	if (element != NODENULL) {
		pen.setColor(Qt::darkGray);
		painter.setPen(pen);
		QPolygon triangle;
		painter.drawRect(QRect(QPoint(150, 40), QPoint(170, 120)));     // 画矩形
		if (get.movement == NODEOUT) {
			triangle.setPoints(3, 140, 40, 160, 20, 180, 40);           // 三点坐标
			painter.drawText(QPoint(172, 60), QString("出队"));
		}
		else if (get.movement == NODEIN) {
			triangle.setPoints(3, 140, 120, 160, 135, 180, 120);        // 三点坐标
			painter.drawText(QPoint(172, 60), QString("入队"));
		}
		painter.drawPolygon(triangle);                                  // 画三角形
		pen.setColor(Qt::red);
		painter.setPen(pen);
		painter.drawEllipse(QPoint(160, 145), 10, 10);
		painter.drawText(QPoint(155, 148), QString("%1").arg(element));
	}

	pix.save("./image/queue.png");

	return feedback;
}


/************************************************ tool ************************************************/
//@name : splitString
//@func : 自定义提取string串中数字
//@time : 2022.8.27
void splitString(vector<string>& strings, const string& org_string, const string& seq) {
	string::size_type p1 = 0;
	string::size_type p2 = org_string.find(seq);

	while (p2 != string::npos) {
		if (p2 == p1) {
			++p1;
			p2 = org_string.find(seq, p1);
			continue;
		}
		strings.push_back(org_string.substr(p1, p2 - p1));
		p1 = p2 + seq.size();
		p2 = org_string.find(seq, p1);
	}

	if (p1 != org_string.size()) {
		strings.push_back(org_string.substr(p1));
	}
}

