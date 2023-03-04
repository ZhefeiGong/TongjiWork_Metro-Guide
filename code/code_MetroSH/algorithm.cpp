//@time   : 2022.8.30
//@author : gonzalez
//@func   : 内部执行函数


#include <iostream>
#include <fstream>
#include <algorithm>
#include <queue>
#include "algorithm.h" // 算法类
#include "gonStack.h"  // 自定义栈
#include "gonQueue.h"  // 自定义队列


/*********************************** 路线数据类 ***********************************/
// @name   : routeInfo
// @time   : 2022.8.31
// @func   : 构造函数
routeInfo::routeInfo()
{
	// 构造初始化
	this->id = DEFAULT_LINE_ID;
	this->name = DEFAULT_LINE_NAME;
	this->num = DEFAULT_LIEN_NUM;
	this->origin = DEFAULT_LINE_ORIGIN;
	this->destination = DEFAULT_LINE_DESTINATION;
}
// @name   : ~routeInfo
// @time   : 2022.8.31
// @func   : 析构函数
routeInfo::~routeInfo()
{
	// none
}
// @name   : setId
// @time   : 2022.8.31
// @func   : 
bool routeInfo::setId(int myId)
{
	this->id = myId;
	return true;
}
// @name   : setNamae
// @time   : 2022.8.31
// @func   : 
bool routeInfo::setName(string myName)
{
	this->name = myName;
	return true;
}
// @name   : setNum
// @time   : 2022.8.31
// @func   : 
bool routeInfo::setNum(int myNum)
{
	this->num = myNum;
	return true;
}
// @name   : setOrigin
// @time   : 2022.8.31
// @func   : 
bool routeInfo::setOrigin(string myOrigin)
{
	this->origin = myOrigin;
	return true;
}
// @name   : setDestination
// @time   : 2022.8.31
// @func   : 
bool routeInfo::setDestination(string myDestination)
{
	this->destination = myDestination;
	return true;
}
// @name   : setColor
// @time   : 2022.9.2
// @func   : 
bool routeInfo::setColor(QColor myColor)
{
	this->color = myColor;
	return true;
}
// @name   : addSite
// @time   : 2022.8.31
// @func   : 在indexHead和indexTail之间插入站点信息
bool routeInfo::insertSite(routeSiteInfo mySite, string Head, string Tail)
{
	if (Head == "NULL") {
		if (Tail == this->sites[0].name) {
			this->sites.insert(this->sites.begin(), mySite);          // 开头添加新元素
			this->num++;
			this->origin = mySite.name;
			return true;
		}
		else if (Tail == this->sites[this->sites.size() - 1].name) {
			this->sites.insert(this->sites.end(), mySite);            // 末尾追加新元素
			this->num++;
			this->destination = mySite.name;
			return true;
		}
	}
	else if (Tail == "NULL") {
		if (Head == this->sites[0].name) {
			this->sites.insert(this->sites.begin(), mySite);          // 开头添加新元素
			this->num++;
			this->origin = mySite.name;
			return true;
		}
		else if (Head == this->sites[this->sites.size() - 1].name) {
			this->sites.insert(this->sites.end(), mySite);            // 末尾追加新元素
			this->num++;
			this->destination = mySite.name;
			return true;
		}
	}
	else {
		int indexBefore = 0;
		int indexAfter = 0;
		for (int i = 0; i < this->sites.size(); i++) {
			if (Head == this->sites[i].name) {
				indexBefore = i;
			}
			else if (Tail == this->sites[i].name) {
				indexAfter = i;
			}
		}
		if (abs(indexBefore - indexAfter) == 1) {
			int indexMax = (indexBefore > indexAfter) ? indexBefore : indexAfter;
			this->sites.insert(this->sites.begin() + indexMax, mySite);
			this->num++;
			return true;
		}

	}
	return false;
}
// @name   : deleteSite
// @time   : 2022.9.3
// @func   : 删除name站点
bool routeInfo::deleteSite(string name)
{
	// 删除该值
	for (auto it = this->sites.begin(); it != this->sites.end();) {
		if ((*it).name == name) {
			// 更新origin和destination
			if (name == this->origin) {
				if (this->sites.size() >= 2) {
					this->origin = this->sites[1].name;
				}
				else {
					this->origin = "NULL";
				}
			}
			else if(name == this->destination){
				if (this->sites.size() >= 2) {
					this->destination = this->sites[this->sites.size() - 2].name;
				}
				else {
					this->destination = "NULL";
				}
			}
			// 更新num
			this->num--;
			// 删除
			it = this->sites.erase(it);
			break;
		}
		else {
			++it;
		}
	}
	
	return true;
}
// @name   : addSite
// @time   : 2022.8.31
// @func   : 
bool routeInfo::pushSite(routeSiteInfo mySite)
{
	this->sites.push_back(mySite);
	return true;
}
// @name   : getId
// @time   : 2022.8.31
// @func   : 
int routeInfo::getId(void)
{
	return this->id;
}
// @name   : getName
// @time   : 2022.8.31
// @func   : 
string routeInfo::getName(void)
{
	return this->name;
}
// @name   : getNum
// @time   : 2022.8.31
// @func   : 
int routeInfo::getNum(void)
{
	return this->num;
}
// @name   : getOrigin
// @time   : 2022.8.31
// @func   : 
string routeInfo::getOrigin(void)
{
	return this->origin;
}
// @name   : getDestination
// @time   : 2022.8.31
// @func   : 
string routeInfo::getDestination(void)
{
	return this->destination;
}
// @name   : getSite
// @time   : 2022.8.31
// @func   : 获取该线路中的站点信息
// @notice : index为该条站点信息的索引值
routeSiteInfo routeInfo::getSite(int index)
{
	return this->sites[index];
}
// @name   : getColor
// @time   : 2022.9.2
// @func   : 
QColor routeInfo::getColor(void) {
	return this->color;
}
// @name   : show
// @time   : 2022.8.31
// @func   : 
void routeInfo::show(void)
{
	// 基本情况展现
	cout << "id      : " << this->id << endl;
	cout << "name    : " << this->name << endl;
	cout << "num     : " << this->num << endl;
	cout << "fromTo  : " << this->origin << "->" << this->destination << endl;
	// 显示站点信息
	for (int i = 0; i < this->num; i++) {
		cout << sites[i].name << ' ' << sites[i].latitude << ' ' << sites[i].longitude << endl;
	}

	return;
}

/*********************************** 内部执行算法类 ***********************************/
// @name   : getSite
// @time   : 2022.8.31
// @func   : 
subwayAlgorithm::subwayAlgorithm()
{
	this->routes.clear();
	this->lines.clear();
	this->sites.clear();

	this->linesHashMap.clear();
	this->routesHashMap.clear();
	this->sitesHashMap.clear();

	this->changesMinAns.clear();
	this->changesMinAns_lines.clear();
	this->timeMinAns.clear();
}
// @name   : ~subwayAlgorithm
// @time   : 2022.8.31
// @func   : 
subwayAlgorithm::~subwayAlgorithm()
{
	// ...none...
}
// @name   : getRoutesFromFile
// @time   : 2022.8.31
// @func   : 从文件中读取原始route数据
bool subwayAlgorithm::getRoutesFromFile(bool isStandard)
{
	ifstream inFile;
	if (isStandard) {
		inFile.open(FILENAME_STANDARD);
	}
	else {
		inFile.open(FILENAME);
	}
	string name_ = "name";
	string numSite_ = "numSite";
	string colour_ = "colour";
	string fromTo_ = "fromTo";
	string continueSign = "----------";
	string endSign = "==========";

	int idCount = 1;

	while (true) {
		// route初始化
		routeInfo route;
		route.setId(idCount);
		idCount++;
		int num;
		string temp, name, origin, destination;
		// name获取
		inFile >> temp;
		if (temp == name_) {
			inFile >> name;
			route.setName(name);
		}
		else{
			return false;
		}
		// numSite获取
		inFile >> temp;
		if (temp == numSite_) {
			inFile >> num;
			route.setNum(num);
		}
		else {
			return false;
		}
		// color初始化
		inFile >> temp;
		if (temp == colour_) {
			inFile >> temp;
			QString  str = QString::fromStdString(temp);
			int value = str.toInt(nullptr, 16);
			route.setColor(QColor(QRgb(value)));
		}
		else {
			return false;
		}
		// fromTo获取
		inFile >> temp;
		if (temp == fromTo_) {
			inFile >> origin;
			inFile >> destination;
			route.setOrigin(origin);
			route.setDestination(destination);
		}
		else {
			return false;
		}
		// 获取站点信息
		for (int i = 0; i < route.getNum(); i++) {
			routeSiteInfo site;
			inFile >> site.name;
			inFile >> site.latitude;
			inFile >> site.longitude;
			route.pushSite(site);
		}
		// 哈希表存储
		this->routesHashMap.insert(make_pair(route.getName(), (int)this->routes.size()));
		// 数据存储
		this->routes.push_back(route);
		// 判断是否结束
		inFile >> temp;
		if (temp == continueSign) {
			continue;  // 开始循环
		}
		else if (temp == endSign) {
			break;     // 结束循环
		}
	}
	inFile.close();

	return true;

}
// @name   : routes2routesHash
// @time   : 2022.9.3
// @func   : 数据到哈希表
void subwayAlgorithm::routes2routesHash(void)
{
	for (int i = 0; i < this->routes.size(); i++) {
		this->routesHashMap.insert(make_pair(this->routes[i].getName(), i));
	}
	return;
}
// @name   : routes2sites
// @time   : 2022.8.31
// @func   : 根据每个route提取站点数据
void subwayAlgorithm::routes2sites(void)
{
	for (int i = 0; i < this->routes.size(); i++) {
		string lineName = this->routes[i].getName();
		// 遍历每个route的site
		for (int j = 0; j < this->routes[i].getNum(); j++) {
			routeSiteInfo routeSite = this->routes[i].getSite(j);                                    // 尝试遍历的点
			unordered_map<string, int>::const_iterator it = this->sitesHashMap.find(routeSite.name); // 哈希表寻找
			// 已添加该结点
			if (it != this->sitesHashMap.end()) {
				int index = it->second;
				// 经过路线名
				this->sites[index].viaLines.push_back(lineName);
				// 添加邻接结点
				if (j > 0) {
					routeSiteInfo routeSiteBefore = this->routes[i].getSite(j - 1);
					// 判断之前是否出现
					if (count(this->sites[index].adjacentNodes.begin(), this->sites[index].adjacentNodes.end(), routeSiteBefore.name) == 0) {
						this->sites[index].adjacentNodes.push_back(routeSiteBefore.name);
					}
				}
				if (j < this->routes[i].getNum() - 1) {
					routeSiteInfo routeSiteAfter = this->routes[i].getSite(j + 1);
					// 判断之前是否出现
					if (count(this->sites[index].adjacentNodes.begin(), this->sites[index].adjacentNodes.end(), routeSiteAfter.name) == 0) {
						this->sites[index].adjacentNodes.push_back(routeSiteAfter.name);
					}
				}
			}
			// 未添加该结点
			else{
				siteInfo site;
				// 信息初始化
				site.name = routeSite.name;
				site.latitude = routeSite.latitude;
				site.longitude = routeSite.longitude;
				site.viaLines.push_back(lineName);
				// 添加邻接结点
				if (j > 0) {
					routeSiteInfo routeSiteBefore = this->routes[i].getSite(j - 1);
					site.adjacentNodes.push_back(routeSiteBefore.name);
				}
				if (j < this->routes[i].getNum() - 1) {
					routeSiteInfo routeSiteAfter = this->routes[i].getSite(j + 1);
					site.adjacentNodes.push_back(routeSiteAfter.name);
				}
				// 结点添加进入列表
				this->sites.push_back(site);
				// 哈希表添加
				this->sitesHashMap.insert(make_pair(site.name, (int)(this->sites.size()-1)));
			}
		}
	}
	return;
}
// @name   : routes2lines
// @time   : 2022.9.1
// @func   : 构造路线邻接矩阵
void subwayAlgorithm::routes2lines(void)
{
	// lines赋初值
	for (int i = 0; i < this->routes.size(); i++) {
		lineInfo line;                                      // 初始化
		line.name = this->routes[i].getName();              // 路线名赋值
		this->linesHashMap.insert(make_pair(line.name, i)); // 哈希表添加
		this->lines.push_back(line);                        // 添加路线
	}
	// 路线链接
	for (int i = 0; i < this->sites.size(); i++) {
		for (int j = 0; j < this->sites[i].viaLines.size(); j++) {
			string name = this->sites[i].viaLines[j];
			int nameIndex = this->linesHashMap.find(name)->second;
			for (int k = 0; k < this->sites[i].viaLines.size(); k++) {
				if (k != j) {
					if (count(this->lines[nameIndex].lines.begin(), this->lines[nameIndex].lines.end(), this->sites[i].viaLines[k]) == 0) {
						this->lines[nameIndex].lines.push_back(this->sites[i].viaLines[k]);  // 添加对应路线
					}
				}
			}
		}
	}
	return;
}
// @name   : changesMinSerach
// @time   : 2022.8.31
// @func   : 寻找地铁换乘最少路径
// @notice : 广度优先搜索
void subwayAlgorithm::changesMinSerach(string start, string end)
{	
	// 初始化
	vector<string> startLines = this->sites[this->sitesHashMap.find(start)->second].viaLines;
	vector<string> endLines = this->sites[this->sitesHashMap.find(end)->second].viaLines;
	vector<vector<string>> ans;
	vector<int> rank;

	// 获取最少换乘点
	for (int i = 0; i < startLines.size(); i++) {
		for (int j = 0; j < endLines.size(); j++) {
			vector<string> temp = linesChangesMinSerach(startLines[i], endLines[j]);
			ans.push_back(temp);
			rank.push_back((int)temp.size());
		}
	}
	int minPosition = (int)(min_element(rank.begin(), rank.end()) - rank.begin());
	vector<string>lineAns = ans[minPosition];
	this->changesMinAns_lines = lineAns;

	// 开始提取站点并存入changesMinAns中
	vector<string>keyNode;
	keyNode.push_back(start);
	for (int i = 0; i < lineAns.size() - 1; i++) {
		int index = this->routesHashMap.find(lineAns[i])->second;
		for (int j = 0; j < this->routes[index].getNum(); j++) {
			routeSiteInfo site = this->routes[index].getSite(j);
			int indexSite = this->sitesHashMap.find(site.name)->second;
			if (count(this->sites[indexSite].viaLines.begin(), this->sites[indexSite].viaLines.end(), lineAns[i+1]) != 0) {
				keyNode.push_back(site.name);
				break;
			}
		}
	}
	keyNode.push_back(end);

	// 提取站点信息
	for (int i = 0; i < lineAns.size(); i++) {
		int routeIndex = this->routesHashMap.find(lineAns[i])->second;
		int siteIndex[2] = { 0,0 };
		for (int j = 0; j < this->routes[routeIndex].getNum(); j++) {
			routeSiteInfo site = this->routes[routeIndex].getSite(j);
			if (site.name == keyNode[i]) {
				siteIndex[0] = j;
			}
			else if (site.name == keyNode[i + 1]) {
				siteIndex[1] = j;
			}
		}
		if (siteIndex[0] > siteIndex[1]) {
			for (int k = siteIndex[0]; k > siteIndex[1]; k--) {
				string name = this->routes[routeIndex].getSite(k).name;
				this->changesMinAns.push_back(name);
			}
		}
		else {
			for (int k = siteIndex[0]; k < siteIndex[1]; k++) {
				string name = this->routes[routeIndex].getSite(k).name;
				this->changesMinAns.push_back(name);
			}
		}
	}
	this->changesMinAns.push_back(end);

	return;
}
// @name   : linesChangesMinSerach
// @time   : 2022.9.1
// @func   : 获取startLine路线至endLine路线的最短历程
vector<string> subwayAlgorithm::linesChangesMinSerach(string startLine, string endLine)
{
	vector<string> ans;

	// 线路一致(特判)
	if (startLine == endLine) {
		ans.push_back(startLine);
	}
	// 路线不同
	else {

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
		while (!queue.empty()) {
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
				int index = this->linesHashMap.find(top)->second;
				for (int i = 0; i < this->lines[index].lines.size(); i++) {
					bool find = false;
					for (int j = 0; j <= layer; j++) {
						if (count(bfsStore[j].begin(), bfsStore[j].end(), this->lines[index].lines[i]) != 0) {
							find = true;
							break;
						}
					}
					if (!find) {
						bfsStore[layer].push_back(this->lines[index].lines[i]);
						queue.push(this->lines[index].lines[i]);
						if (this->lines[index].lines[i] == endLine) {
							breakSign = true;
							break;
						}
					}
				}
			}
			if (breakSign)
				break;
		}

		// 获得路径信息
		string get = endLine;
		ans.push_back(endLine);
		bool outSign = false;
		for (int i = layer - 1; i >= 0; i--) {
			outSign = false;
			int index = this->linesHashMap.find(get)->second;
			for (int j = 0; j < this->lines[index].lines.size(); j++) {
				string name = this->lines[index].lines[j];
				for (int k = 0; k < bfsStore[i].size(); k++) {
					if (bfsStore[i][k] == name) {
						ans.push_back(name);
						get = name;
						outSign = true;
						break;
					}
				}
				if (outSign) {
					break;
				}
			}
		}

	}

	reverse(ans.begin(), ans.end()); // 逆序

	return ans;
}
// @name   : timeMinSerach
// @time   : 2022.8.31
// @func   : dijkstra算法
void subwayAlgorithm::timeMinSerach(string start, string end)
{
	// 创建
	vector<dijkstraNode> dijkstraList;        // 优先队列
	unordered_map<string, int> dijkstraHash;  // 哈希表
	vector<string>timeMinAnsTemp;             // 存储遍历结果
	// 初始化
	for (int i = 0; i < this->sites.size(); i++) {
		dijkstraNode node;
		node.name = this->sites[i].name;
		if (node.name == start) {
			node.distance = 0;
		}
		dijkstraList.push_back(node);
		dijkstraHash.insert(make_pair(node.name, i));
	}
	// 单源最短路径搜索
	while (true) {
		int minPosition = (int)(min_element(dijkstraList.begin(), dijkstraList.end()) - dijkstraList.begin()); // 求出最短路径点
		timeMinAnsTemp.push_back(dijkstraList[minPosition].name);                                              // 遍历最近点
		// 未至终点
		if (dijkstraList[minPosition].name != end) {
			// 更新周围坐标 
			int index = this->sitesHashMap.find(dijkstraList[minPosition].name)->second;
			for (int i = 0; i < this->sites[index].adjacentNodes.size(); i++) {
				// 判断是否更新
				int listIndex = dijkstraHash.find(this->sites[index].adjacentNodes[i])->second;
				if (!dijkstraList[listIndex].visited) {
					// 求新distance
					int tempIndex = this->sitesHashMap.find(this->sites[index].adjacentNodes[i])->second;
					double dis_latitude = pow((this->sites[index].latitude * EXPANDTIMES - this->sites[tempIndex].latitude * EXPANDTIMES), 2);
					double dis_longitude = pow((this->sites[index].longitude * EXPANDTIMES - this->sites[tempIndex].longitude * EXPANDTIMES), 2);
					int newDistance = (int)sqrt(dis_latitude + dis_longitude) + dijkstraList[minPosition].distance;
					// 更新distance
					int dijkstraIndex = dijkstraHash.find(this->sites[tempIndex].name)->second;
					if (dijkstraList[dijkstraIndex].distance > newDistance) {
						dijkstraList[dijkstraIndex].distance = newDistance;
					}
				}
			}
			// 清除该点
			dijkstraList[minPosition].distance = INT_MAX;
			dijkstraList[minPosition].visited = true;
		}
		// 已至终点
		else{
			break;
		}
	}
	// 结果回溯
	string name = timeMinAnsTemp.back();
	int index = this->sitesHashMap.find(name)->second;
	this->timeMinAns.push_back(name);
	for (int i = (int)timeMinAnsTemp.size() - 1; i >= 0; i--) {
		if (count(this->sites[index].adjacentNodes.begin(), this->sites[index].adjacentNodes.end(), timeMinAnsTemp[i]) != 0) {
			this->timeMinAns.push_back(timeMinAnsTemp[i]);
			name = timeMinAnsTemp[i];
			index = this->sitesHashMap.find(name)->second;
		}
	}
	reverse(this->timeMinAns.begin(), this->timeMinAns.end()); // 回溯后逆序
	return;
}
// @name   : clearAns
// @time   : 2022.9.3
// @func   : 清除结果数据
void subwayAlgorithm::clearAns(void)
{
	this->changesMinAns.clear();
	this->changesMinAns_lines.clear();
	this->timeMinAns.clear();
	return;
}
// @name   : clear
// @time   : 2022.9.3
// @func   : 清除原始数据
void subwayAlgorithm::clear(void)
{
	// 原始数据
	this->sites.clear();                   // 站点信息的邻接列表
	this->lines.clear();                   // 路线信息的邻接列表
	this->routesHashMap.clear();           // 路线名字与index的哈希表
	this->sitesHashMap.clear();            // 站点名字与index的哈希表
	this->linesHashMap.clear();            // 路线名字与index的哈希表

	// 结果数据
	this->changesMinAns_lines.clear();       // 换乘最少的路线变化情况
	this->changesMinAns.clear();             // 换乘最少--BFS
	this->timeMinAns.clear();                // 时间最短--迪杰斯特拉
}


//-------------------------------- FOR TEST --------------------------------
// @name   : showRoutes
// @time   : 2022.8.31
// @func   : 显示路线信息
void subwayAlgorithm::showRoutes(void) 
{
	for (int i = 0; i < this->routes.size(); i++){
		this->routes[i].show();
	}
	cout << "总路线数 : " << this->routes.size() << endl;

	return;
}
// @name   : showSites
// @time   : 2022.8.31
// @func   : 显示站点信息
void subwayAlgorithm::showSites(void)
{
	for (int i = 0; i < this->sites.size(); i++) {

		cout << "----------" << endl;
		cout << this->sites[i].name << endl;
		cout << this->sites[i].latitude << ' ' << this->sites[i].longitude << endl;
		for (int j = 0; j < this->sites[i].viaLines.size(); j++) {
			cout << this->sites[i].viaLines[j] << ' ';
		}
		cout << endl;
		for (int k = 0; k < this->sites[i].adjacentNodes.size(); k++) {
			cout << this->sites[i].adjacentNodes[k] << ' ';
		}
		cout << endl;
	}
	cout << "总站点数 : " << this->sites.size() << endl;

	return;
}
// @name   : showLines
// @time   : 2022.8.31
// @func   : 显示路线规划
void subwayAlgorithm::showLines(void)
{
	for (int i = 0; i < this->lines.size(); i++) {
		cout << this->lines[i].name << "->";
		for (int j = 0; j < this->lines[i].lines.size(); j++) {
			cout << this->lines[i].lines[j] << " | ";
		}
		cout << endl;
	}
	return;
}
// @name   : showChangesMinAns
// @time   : 2022.8.31
// @func   : 
void subwayAlgorithm::showChangesMinAns(void)
{
	cout << "路线换乘:";
	for (int i = 0; i < this->changesMinAns_lines.size(); i++) {
		cout << this->changesMinAns_lines[i];
		if (i != this->changesMinAns_lines.size() - 1) {
			cout << "->";
		}
	}
	cout << endl;
	cout << "站点换乘:";
	for (int i = 0; i < this->changesMinAns.size(); i++) {
		cout << this->changesMinAns[i];
		if (i != this->changesMinAns.size() - 1) {
			cout << "->";
		}
	}
	cout << endl;
}
// @name   : showTimeMinAns
// @time   : 2022.8.31
// @func   : 
void subwayAlgorithm::showTimeMinAns(void)
{
	cout << "站点换乘:";
	for (int i = 0; i < this->timeMinAns.size(); i++) {
		cout << this->timeMinAns[i];
		if (i != this->timeMinAns.size() - 1) {
			cout << "->";
		}
	}
	cout << endl;
}


