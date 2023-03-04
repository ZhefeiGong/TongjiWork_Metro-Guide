//@timae  : 2022.8.25 - 2022.8.29
//@author : gonzalez
//@info   : 实现内部算法(头文件)
#pragma once

#include <iostream>
#include <string.h>
using namespace std;

//@func : 宏定义
#define NODENULL -1
#define NAMENULL "null"
#define NODEIN   1
#define NODEOUT  -1
#define DFS_NONRECUR    1
#define DFS_RECUR       2
#define BFS_NONRECUR    3

typedef int gonTYPE;

/***************************************** gonStack类 *****************************************/
//@name : gonStack栈类
//@func : 自行实现栈
//@time : 2022.8.26
typedef struct stackNode {
	gonTYPE value = NODENULL;
	stackNode* next = NULL;
}stackNode,*stackPointer;

class gonStack {
protected:
	stackPointer stackTop = NULL;
	stackPointer stackHead = NULL;
	int length = 0;
public:
	gonStack();   // 栈构造函数
	~gonStack();  // 栈析构函数
	bool empty(void);
	gonTYPE top(void);
	bool pop(void);
	bool push(gonTYPE tempValue);
	int size(void);
};


/***************************************** gonQueue类 *****************************************/
//@name : gonQueue队列类
//@func : 自行实现队列
//@time : 2022.8.26
typedef struct queueNode {
	queueNode* before = NULL;
	gonTYPE value = NODENULL;
	queueNode* after = NULL;
}queueNode, *queuePointer;

class gonQueue {
protected:
	queuePointer queueHead = NULL;
	queuePointer queueTail = NULL;
	int length = 0;
public:
	gonQueue();   // 队列构造函数
	~gonQueue();  // 队列析构函数
	bool empty(void);
	gonTYPE front(void);
	gonTYPE back(void);
	bool pop(void);
	bool push(gonTYPE tempValue);
	int size(void);
};


/***************************************** moveRecord类 *****************************************/
//@name : moveNode
//@func : 每次移动的步骤
//@time : 2022.8.26
typedef struct moveNode {
	int nodeIndex = NODENULL;           // 移动的顶点
	int movement = 0;                   // 本次移动是移出/移进
	moveNode* next = NULL;              // 下一次移动步骤
}moveNode, * movePointer;

class moveRecord {
protected:
	moveNode start;
public:
	void push(int index, int movement);   // 数据进栈函数
	moveNode pop(void);                   // 弹出第一个元素并返回
	moveNode top(void);                   // 返回第一个元素
	void clear(void);                     // 清除函数
	void show(void);                      // 展现栈/队列移动结果
	~moveRecord();                        // 析构函数
};


/***************************************** matrixGraph类 *****************************************/
//@name : adjacentListNode
//@func : 邻接链表节点
//@time : 2022.8.26
typedef struct adjacentListNode {
	int nodeIndex = NODENULL;                  // 顶点索引
	adjacentListNode* next = NULL;             // 邻接链表下一个值
}adjacentListNode, *adjacentListNodePointer;

//@name   : matrixGraph
//@func   : 矩阵-图
//@time   : 2022.8.26
//@notcie : ①顶点name从0开始计算 并对应其index值
//          ②以无向图形式展示
class matrixGraphAlgorithm{
public:
	int numVertex;                                   // 顶点个数
	string* nameVertex;                              // 顶点名字(此项目中可能无需使用)
	bool** adjacentMatrix;                           // 邻接矩阵
	adjacentListNodePointer adjacentList;            // 邻接列表
	int* indexAnsDfs;                                // 深度优先搜索遍历结果
	int* indexAnsBfs;                                // 广度优先搜索遍历结果
	moveRecord moveDfs;                              // 移动的步骤
	moveRecord moveBfs;                              // 移动的步骤

public:
	matrixGraphAlgorithm();                       // 构造函数
	~matrixGraphAlgorithm();                      // 析构函数

	void initiate(int num);                       // 初始化
	void clear(void);                             // 全局清除
	void buildMatrixAll(bool* tempMatrix);        // 直接建立所有邻接矩阵
	void buildMatrixEach(int indexA, int indexB); // 根据index值更新Matrix
	void matrixUpdataList(void);                  // 根据邻接矩阵更新邻接列表
	// 图搜索过程记录
	bool visited(int index, int method);          // 深度搜索过程中判断是否搜索过
	bool ansAdd(int index, int method);           // 添加遍历结果
	void clearAns(void);                          // 清除结果值
	// 图深度优先搜索
	void dfsRecursion(int index);                 // 递归深度优先搜索
	void dfsNonRecursion(int index);              // 非递归深度优先搜索
	// 图广度优先搜索
	void bfsNonRecursion(int index);              // 非递归广度优先搜索


	/******************test******************/
	void showMatrix(void);
	void showList(void);
	void showAns(void);
	
};








