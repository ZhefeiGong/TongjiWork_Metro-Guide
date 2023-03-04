//@time   : 2022.8.30
//@author : gonzalez
//@func   : 地铁项目内部执行函数

#pragma once
#include <string>        // string类
#include <vector>        // vector容器
#include <qcolor.h>      // qt颜色
#include <unordered_map> // 哈希表


using namespace std;


/*********************************** 路线数据类 ***********************************/
// @name  : routeSiteInfo
// @func  : 存储路线站点信息
#define DEFAULT_NAME       "null"
#define DEFAULT_LONGITUDE     -1
#define DEFAULT_LATITUDE      -1
typedef struct routeSiteInfo {
	string name = DEFAULT_NAME;               // 站点名
	double longitude =  DEFAULT_LONGITUDE;    // 站点精度
	double latitude = DEFAULT_LATITUDE;       // 站点纬度
}routeSiteInfo, * routeSiteInfoPointer;
// @name   : routeInfo
// @func   : 路线信息类
// @notice : 该类将从地铁txt文件中获取原始数据 
#define DEFAULT_LINE_ID            0
#define DEFAULT_LINE_NAME          "null"
#define DEFAULT_LIEN_NUM           0
#define DEFAULT_LINE_ORIGIN        "null"
#define DEFAULT_LINE_DESTINATION   "null"
class routeInfo {
private:
	int id;                       // 路线id
	string name;                  // 路线名
	int num;                      // 路线站点个数
	string origin;                // 路线起点名
	string destination;           // 路线终点名
	vector<routeSiteInfo> sites;  // 各个站点信息
	QColor color;                 // 该路线颜色

public:

	// 基本函数
	routeInfo();                  // 构造函数
	~routeInfo();                 // 析构函数

	// 初始化设置信息
	bool setId(int myId);
	bool setName(string myName);
	bool setNum(int myNum);
	bool setOrigin(string myOrigin);
	bool setDestination(string myDestination);
	bool setColor(QColor mycolor);
	bool pushSite(routeSiteInfo mySite);

	// 添加站点
	bool insertSite(routeSiteInfo mySite, string Head, string Tail);
	// 删除站点
	bool deleteSite(string name);

	// 获取信息
	int getId(void);
	string getName(void);
	int getNum(void);
	string getOrigin(void);
	string getDestination(void);
	routeSiteInfo getSite(int index);
	QColor getColor(void);

	// 打印展示数据
	void show(void);

};

/*********************************** 内部实现类 ***********************************/
// @name   : siteInfo
// @func   : 站点信息
// @notice : 用于邻接链表遍历的站点信息
typedef struct siteInfo {
	string name = DEFAULT_NAME;               // 站点名
	double longitude = DEFAULT_LONGITUDE;     // 站点精度
	double latitude = DEFAULT_LATITUDE;       // 站点纬度
	vector<string> viaLines;                  // 站点途径的路线信息
	vector<string> adjacentNodes;             // 邻接站点信息
}siteInfo, *siteInfoPointer;

// @name   : lineInfo
// @func   : 路线邻接队列
// @notice : 
typedef struct lineInfo {
	string name;
	vector<string> lines;
}lineInfo, * lineInfoPointer;

// @name   : dijkstraNode
// @func   : 迪杰斯特拉结点
// @notice : 
#define EXPANDTIMES 1000
typedef struct dijkstraNode {
	string name;
	int distance = INT_MAX;
	bool visited = false;
	// ' > '重载
	bool operator>(const dijkstraNode& another)const
	{
		return this->distance > another.distance;
	}
	// ' < '重载
	bool operator<(const dijkstraNode& another)const
	{
		return this->distance < another.distance;
	}
}dijkstraNode, *dijkstraNodePointer;


// @name   : subwayAlgorithm
// @func   : 地铁内部执行类
// @notice : 地铁内部搜索类
#define FILENAME             "src/data/subwayInfo.txt"
#define FILENAME_STANDARD    "src/data/subwayInfo_standard.txt"
#define TIMEMIN              1
#define CHANGESMIN           2
class subwayAlgorithm {
protected:
	friend class MetroSH;                     // 声明友元类

	// 地铁信息数据
	vector<routeInfo> routes;                 // 路线信息
	vector<siteInfo> sites;                   // 站点信息的邻接列表
	vector<lineInfo> lines;                   // 路线信息的邻接列表
	unordered_map<string, int>routesHashMap;  // 路线名字与index的哈希表
	unordered_map<string, int>sitesHashMap;   // 站点名字与index的哈希表
	unordered_map<string, int>linesHashMap;   // 路线名字与index的哈希表

	// 查找结果数据
	vector<string> changesMinAns_lines;       // 换乘最少的路线变化情况
	vector<string> changesMinAns;             // 换乘最少--BFS
	vector<string> timeMinAns;                // 时间最短--迪杰斯特拉

public:
	subwayAlgorithm();                               // 构造函数
	~subwayAlgorithm();                              // 析构函数

	bool getRoutesFromFile(bool isStandard = false); // 从原始文件获取地铁原始数据

	void routes2sites(void);                         // 根据路线信息提取数据存入sites中
	void routes2lines(void);                         // 根据路线信息提取数据存入lines中
	void routes2routesHash(void);                    // 根据路线信息提取对应的哈希表

	// 换乘最少 -- 返回路线(深度优先搜索 - 优先深入上次结点所在线路)
	void changesMinSerach(string start, string end);                        // 获取换乘最少路线
	vector<string> linesChangesMinSerach(string startLine, string endLine); // 获取路线最近抵达

	// 时间最少 -- 返回路线(广度优先搜索 - 回溯算法是关键)
	void timeMinSerach(string start, string end);                           // 获取时间最短路线

	void clearAns(void);                                                    // 清除ANS数据
	void clear(void);                                                       // 除routes数据均清除

	//-------- FOR TEST --------
	void showRoutes(void);
	void showSites(void);
	void showLines(void);
	void showChangesMinAns(void);
	void showTimeMinAns(void);

};
