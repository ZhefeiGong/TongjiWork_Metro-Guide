//@timae  : 2022.8.25 - 2022.8.29
//@author : gonzalez
//@func   : 实现内部算法(实现文件)
#include "algorithm.h"
using namespace std;


/***************************************** matrixGraph类 *****************************************/
//@name : matrixGraph
//@func : 构造函数
//@time : 2022.8.26
matrixGraphAlgorithm::matrixGraphAlgorithm()
{
	this->indexAnsDfs = NULL;
	this->indexAnsBfs = NULL;
	this->adjacentList = NULL;
	this->adjacentMatrix = NULL;
	this->nameVertex = NULL;
	this->numVertex = 0;
	return;
}
//@name : initiate
//@func : 构造函数
//@time : 2022.8.28
void matrixGraphAlgorithm::initiate(int num)
{
	// 初始化顶点个数
	this->numVertex = num;

	// 初始化顶点名字--(暂时无需存储)
	this->nameVertex = new string[this->numVertex];
	for (int i = 0; i < this->numVertex; i++)
		this->nameVertex[i] = NAMENULL;

	// 初始化邻接矩阵--初始化全为false
	this->adjacentMatrix = new bool* [this->numVertex];
	for (int i = 0; i < this->numVertex; i++) {
		this->adjacentMatrix[i] = new bool[this->numVertex];
		for (int j = 0; j < this->numVertex; j++)
			this->adjacentMatrix[i][j] = false;
	}

	// 初始化邻接列表
	this->adjacentList = new adjacentListNode[this->numVertex];     // adjacentList
	for (int i = 0; i < this->numVertex; i++)
		this->adjacentList[i].nodeIndex = i;

	// 初始化结果结果数组
	this->indexAnsDfs = new int[this->numVertex];
	this->indexAnsBfs = new int[this->numVertex];
	for (int i = 0; i < this->numVertex; i++) {
		this->indexAnsBfs[i] = NODENULL;
		this->indexAnsDfs[i] = NODENULL;
	}
}
//@name : clear
//@func : 清除函数
//@time : 2022.8.28
void matrixGraphAlgorithm::clear()
{
	// 修改num值
	this->numVertex = 0;
	// 释放名字nameVertex
	delete[] this->nameVertex;
	this->nameVertex = NULL;
	// 释放空间indexAns
	delete[] this->indexAnsBfs;
	delete[] this->indexAnsDfs;
	this->indexAnsBfs = NULL;
	this->indexAnsDfs = NULL;
	// 释放空间Matrix
	for (int i = 0; i < this->numVertex; i++) {
		delete[] this->adjacentMatrix[i];
	}
	delete this->adjacentMatrix;
	this->adjacentMatrix = NULL;
	// 释放空间List
	for (int i = 0; i < this->numVertex; i++) {
		adjacentListNodePointer temp = this->adjacentList[i].next;
		this->adjacentList[i].next = NULL;
		while (temp != NULL) {
			adjacentListNodePointer temptemp = temp->next;
			delete temp;
			temp = temptemp;
		}
	}
	delete[] this->adjacentList;
	this->adjacentList = NULL;
	// 清除moveRecord值
	this->moveBfs.clear();
	this->moveDfs.clear();

}
//@name : buildMatrixAll
//@func : 矩阵全局更新
//@time : 2022.8.26
void matrixGraphAlgorithm::buildMatrixAll(bool* tempMatrix)
{
	// 直接全局邻接矩阵赋值
	for (int i = 0; i < this->numVertex; i++) {
		for (int j = 0; j < this->numVertex; j++) {
			this->adjacentMatrix[i][j] = *(tempMatrix + i * this->numVertex + j);
		}
	}
	// 基于无向图实现-->转化为无向图矩阵
	for (int i = 0; i < this->numVertex; i++) {
		for (int j = 0; j < this->numVertex; j++) {
			bool ans = this->adjacentMatrix[i][j] || this->adjacentMatrix[j][i];
			this->adjacentMatrix[i][j] = ans;
			this->adjacentMatrix[j][i] = ans;
		}
	}
	return;
}
//@name   : buildMatrixEach
//@func   : 矩阵局部更新
//@time   : 2022.8.26
//@notice : 输入参数为index值(0-...)
void matrixGraphAlgorithm::buildMatrixEach(int indexA, int indexB)
{
	this->adjacentMatrix[indexA][indexB] = true;
	this->adjacentMatrix[indexB][indexA] = true;

	return;
}
//@name : matrixUpdataList
//@func : 根据矩阵更新邻接列表
//@time : 2022.8.26
void matrixGraphAlgorithm::matrixUpdataList(void)
{
	// adjacentList
	for (int i = 0; i < this->numVertex; i++) {
		// 遍历每个graph行
		for (int j = 0; j < this->numVertex; j++) {
			if (this->adjacentMatrix[i][j] == true) {
				adjacentListNodePointer temp = &(this->adjacentList[i]);
				while (temp!=NULL && temp->next != NULL) {
					temp = temp->next;
				}
				temp->next = new adjacentListNode;
				temp->next->nodeIndex = j;
			}
		}
	}
	return;
}
//@name : clearAns
//@func : 清除结果ANS
//@time : 2022.8.26
void matrixGraphAlgorithm::clearAns(void)
{
	// 清除indexAnsDfs/indexAnsBfs
	for (int i = 0; i < this->numVertex; i++) {
		this->indexAnsDfs[i] = NODENULL;
	}
	for (int i = 0; i < this->numVertex; i++) {
		this->indexAnsBfs[i] = NODENULL;
	}
	// 清除moveDfs/moveBfs
	this->moveDfs.clear();
	this->moveBfs.clear();
	return;
}
//@name : dfsRecursion
//@func : 递归实现深度优先搜索
//@time : 2022.8.26
void matrixGraphAlgorithm::dfsRecursion(int index)
{
	// index入栈
	this->moveDfs.push(index, NODEIN);
	// 结果记录
	this->ansAdd(index, DFS_RECUR);
	// index继续深入
	adjacentListNodePointer temp = this->adjacentList[index].next;
	while (temp != NULL) {
		// 未遍历过继续向下搜索
		if (!visited(temp->nodeIndex, DFS_RECUR)){
			this->dfsRecursion(temp->nodeIndex);
		}
		temp = temp->next;
	}
	// index出栈
	this->moveDfs.push(index, NODEOUT);

	return;
}
//@name : dfsNonRecursion
//@func : 非递归实现深度优先搜索
//@time : 2022.8.26
void matrixGraphAlgorithm::dfsNonRecursion(int index)
{
	gonStack stack;                    // 自定义栈
	stack.push(index);                 // 根结点入栈
	this->moveDfs.push(index, NODEIN);

	while (!stack.empty()) {
		// 栈顶更新
		int topIndex = stack.top();
		stack.pop();                           // 出栈
		this->moveDfs.push(topIndex, NODEOUT); // 出栈记录
		// 出栈即遍历
		if (!this->visited(topIndex, DFS_NONRECUR)) {
			this->ansAdd(topIndex, DFS_NONRECUR);  
		}
		// 栈更新
		adjacentListNodePointer temp = this->adjacentList[topIndex].next;
		while (temp != NULL) {
			// 非递归的深度向下递归
			if (!this->visited(temp->nodeIndex, DFS_NONRECUR)){
				stack.push(temp->nodeIndex);                 // 入栈
				this->moveDfs.push(temp->nodeIndex, NODEIN); // 入栈记录
			}
			temp = temp->next;
		}
	}
	return;
}
//@name : bfsNonRecursion
//@func : 非递归实现广度优先搜索
//@time : 2022.8.26
void matrixGraphAlgorithm::bfsNonRecursion(int index)
{
	gonQueue queue;                    // 自定义队列
	queue.push(index);                 // 根结点入队
	this->moveBfs.push(index, NODEIN);
	this->ansAdd(index, BFS_NONRECUR); // 结果记录

	while (!queue.empty()) {
		int topIndex = queue.front();
		queue.pop();                           //出队列
		this->moveBfs.push(topIndex, NODEOUT); //出队记录
		// 队列更新
		adjacentListNodePointer temp = this->adjacentList[topIndex].next;
		while (temp != NULL) {
			// 未搜索的广度向下搜索
			if (!this->visited(temp->nodeIndex,BFS_NONRECUR)) {
				queue.push(temp->nodeIndex);                 // 入队
				this->moveBfs.push(temp->nodeIndex, NODEIN); // 入队记录
				this->ansAdd(temp->nodeIndex, BFS_NONRECUR); // 结果记录
			}
			temp = temp->next;
		}
	}
	return;
}
//@name : visied
//@func : 非递归实现广度优先搜索
//@time : 2022.8.26
bool matrixGraphAlgorithm::visited(int index, int method)
{
	// 递归DFS
	if (method == DFS_RECUR) {
		for (int i = 0; i < this->numVertex; i++) {
			if (this->indexAnsDfs[i] == index) {
				return true;
			}
		}
		return false;
	}
	// 非递归DFS
	else if (method == DFS_NONRECUR) {
		for (int i = 0; i < this->numVertex; i++) {
			if (this->indexAnsDfs[i] == index) {
				return true;
			}
		}
		return false;
	}
	//非递归BFS
	else if (method == BFS_NONRECUR) {
		for (int i = 0; i < this->numVertex; i++) {
			if (this->indexAnsBfs[i] == index) {
				return true;
			}
		}
		return false;
	}
	else
		return false;
}
//@name : ansAdd
//@func : 添加遍历结果
//@time : 2022.8.26
bool matrixGraphAlgorithm::ansAdd(int index, int method)
{
	if (method == DFS_NONRECUR || method == DFS_RECUR) {
		for (int i = 0; i < this->numVertex; i++) {
			if (this->indexAnsDfs[i] == NODENULL) {
				this->indexAnsDfs[i] = index;
				break;
			}
		}
		return true;
	}
	else if (method == BFS_NONRECUR) {
		for (int i = 0; i < this->numVertex; i++) {
			if (this->indexAnsBfs[i] == NODENULL) {
				this->indexAnsBfs[i] = index;
				break;
			}
		}
		return true;
	}
	else
		return false;
}
//@name : ~matrixGraph
//@func : 析构函数
//@time : 2022.8.26
matrixGraphAlgorithm::~matrixGraphAlgorithm()
{
	// 释放空间indexAns
	if (this->indexAnsBfs != NULL) {
		delete[] this->indexAnsBfs;
	}
	if (this->indexAnsDfs != NULL) {
		delete[] this->indexAnsDfs;
	}
	// 释放空间Matrix
	if (this->adjacentMatrix != NULL) {
		for (int i = 0; i < this->numVertex; i++) {
			delete[] this->adjacentMatrix[i];
		}
		delete this->adjacentMatrix;
	}
	// 释放空间List
	if (this->adjacentList != NULL) {
		for (int i = 0; i < this->numVertex; i++) {
			adjacentListNodePointer temp = this->adjacentList[i].next;
			this->adjacentList[i].next = NULL;
			while (temp != NULL) {
				adjacentListNodePointer temptemp = temp->next;
				delete temp;
				temp = temptemp;
			}
		}
		delete[] this->adjacentList;
	}
	// 释放名字nameVertex
	if (this->nameVertex != NULL) {
		delete[] this->nameVertex;
	}
}


/***************************************** gonStack类 *****************************************/
//@name : gonStack
//@func : gonStack类构造函数
//@time : 2022.8.26
gonStack::gonStack()
{
	this->stackHead = new stackNode;
	this->length = 0;
	this->stackTop = this->stackHead;
}
//@name : empty
//@func : 判断栈是否为空(ture为空 | false不为空)
//@time : 2022.8.26
bool gonStack::empty(void)
{
	if (this->length == 0 && this->stackHead == this->stackTop)
		return true;
	else
		return false;
}
//@name : top
//@func : 返回栈顶部的值
//@time : 2022.8.26
gonTYPE gonStack::top(void)
{
	if (!this->empty()) {
		return this->stackTop->value; // 正常返回
	}
	else{
		return false;                 // 栈空
	}
}
//@name : pop
//@func : 栈弹出节点
//@time : 2022.8.26
bool gonStack::pop(void)
{
	if (!this->empty()) {
		stackPointer temp = stackHead;
		while (temp->next != this->stackTop) {
			temp = temp->next;
		}
		delete this->stackTop;         //释放顶部节点
		this->stackTop = temp;         //头指针更新
		this->stackTop->next = NULL;   //头结点更新
		this->length--;                //栈长度更新
		return true;
	}
	else {
		return false;
	}
}
//@name : push
//@func : 栈增添节点
//@time : 2022.8.26
bool gonStack::push(gonTYPE tempValue)
{
	stackPointer temp = new stackNode;  //头结点更新
	this->stackTop->next = temp;        //头结点更新
	this->stackTop = temp;              //头指针更新
	this->length++;                     //栈长度更新
	this->stackTop->value = tempValue;
	return true;
}
//@name : size
//@func : 求得栈长度
//@time : 2022.8.26
int gonStack::size(void)
{
	return this->length;
}
//@name : gonStack
//@func : gonStack类析构函数
//@time : 2022.8.26
gonStack::~gonStack()
{
	while (this->stackHead != this->stackTop) {
		stackPointer temp = this->stackHead->next;
		delete this->stackHead;
		this->stackHead = temp;
	}
	delete this->stackHead;
	this->stackHead = NULL;
	this->stackTop = NULL;
}


/***************************************** gonQueue类 *****************************************/
//@name : gonQueue
//@func : gonQueue类构造函数
//@time : 2022.8.26
gonQueue::gonQueue()
{
	this->length = 0;
}
//@name : empty
//@func : 判断队列是否为空
//@time : 2022.8.26
bool gonQueue::empty(void)
{
	if (this->length == 0 && this->queueHead == NULL && this->queueTail == NULL)
		return true;
	else
		return false;
}
//@name : front
//@func : 返回队列头部值
//@time : 2022.8.26
gonTYPE gonQueue::front(void)
{
	if (!this->empty()) {
		return this->queueHead->value;
	}
	else{
		return false;
	}
}
//@name : back
//@func : 返回队列尾部值
//@time : 2022.8.26
gonTYPE gonQueue::back(void)
{
	if (!this->empty()) {
		return this->queueTail->value;
	}
	else{
		return false;
	}
}
//@name : pop
//@func : 队列头部弹出
//@time : 2022.8.26
bool gonQueue::pop(void)
{
	if (!this->empty()) {
		queuePointer temp = this->queueHead->after;
		if (temp != NULL) {
			temp->before = NULL;
		}
		delete this->queueHead;
		this->queueHead = temp;
		if (this->length == 1) {
			this->queueTail = NULL;
		}
		this->length--;
		return true;
	}
	else{
		return false;
	}
}
//@name : push
//@func : 队列尾部增添
//@time : 2022.8.26
bool gonQueue::push(gonTYPE tempValue)
{
	// 队列为空
	if (this->empty()) {
		this->queueHead = new queueNode;
		this->queueTail = this->queueHead;
		this->queueTail->value = tempValue;
	}
	// 队列尾部添加
	else {
		queuePointer temp = new queueNode;
		this->queueTail->after = temp;
		temp->before = this->queueTail;
		temp->value = tempValue;            //尾部值更新
		this->queueTail = temp;             //队列尾更新
	}
	this->length++;
	return true;

}
//@name : size
//@func : 得到队列长度
//@time : 2022.8.26
int gonQueue::size(void)
{
	return this->length;
}
//@name : ~gonQueue
//@func : gonQueue类析构函数
//@time : 2022.8.26
gonQueue::~gonQueue()
{
	if (!this->empty()) {
		while (this->queueHead != this->queueTail) {
			queuePointer temp = this->queueTail->before;
			delete this->queueTail;
			this->queueTail = temp;
		}
		delete this->queueHead;
		this->queueHead = NULL;
		this->queueTail = NULL;
	}
}


/***************************************** moveRecord类 *****************************************/
//@name : push
//@func : 移动步骤添加
//@time : 2022.8.26
void moveRecord::push(int index, int movement)
{
	// 出栈记录
	movePointer tempMove = &(this->start);
	while (tempMove->next != NULL) {
		tempMove = tempMove->next;
	}
	tempMove->next = new moveNode;
	tempMove = tempMove->next;
	tempMove->nodeIndex = index;
	tempMove->movement = movement;
}
//@name : clear
//@func : 移动步骤清除
//@time : 2022.8.26
void moveRecord::clear(void)
{
	// 清除内容
	movePointer  temp = start.next;
	start.next = NULL;
	start.movement = 0;
	start.nodeIndex = NODENULL;
	while (temp != NULL) {
		movePointer temptemp = temp->next;
		delete temp;
		temp = temptemp;
	}
	return;
}
//@name : ~moveRecord
//@func : 析构函数
//@time : 2022.8.26
moveRecord::~moveRecord()
{
	// 清除内容
	movePointer  temp = start.next;
	start.next = NULL;
	start.movement = 0;
	start.nodeIndex = NODENULL;
	while (temp != NULL) {
		movePointer temptemp = temp->next;
		delete temp;
		temp = temptemp;
	}
}
//@name : show
//@func : cout展现结果
//@time : 2022.8.26
void moveRecord::show(void)
{
	movePointer temp = this->start.next;
	while (temp != NULL) {
		cout << '[' << temp->nodeIndex << ' ' << temp->movement << ']';
		temp = temp->next;
	}
	cout << endl;
}
//@name : pop
//@func : 弹出第一个记录的出入情况
//@time : 2022.8.26
moveNode moveRecord::pop(void)
{
	movePointer temp = this->start.next;
	if (temp != NULL) {
		this->start.next = temp->next; // start指针转换
		moveNode back = *temp;         // 元素暂存
		delete temp;                   // 释放该元素
		return back;
	}
	else {
		return this->start;
	}
}
//@name : top
//@func : 弹出第一个记录的出入情况
//@time : 2022.8.26
moveNode moveRecord::top(void)
{
	movePointer temp = this->start.next;
	if (temp != NULL) {
		return (*temp);
	}
	else {
		return this->start;
	}

}




/********************************************test********************************************/
void matrixGraphAlgorithm::showMatrix(void)
{
	for (int i = 0; i < this->numVertex; i++) {
		for (int j = 0; j < this->numVertex; j++) {
			cout << this->adjacentMatrix[i][j] << ' ';
		}
		cout << endl;
	}
	return;
}
void matrixGraphAlgorithm::showList(void)
{
	for (int i = 0; i < this->numVertex; i++) {
		cout << '[' << i << ']';
		adjacentListNodePointer temp = this->adjacentList[i].next;
		while (temp != NULL) {
			cout << temp->nodeIndex << ' ';
			temp = temp->next;
		}
		cout << endl;
	}
	return;
}
void matrixGraphAlgorithm::showAns(void)
{
	for (int i = 0; i < this->numVertex; i++)
		cout << this->indexAnsDfs[i] << ' ';
	cout << endl;

	for (int i = 0; i < this->numVertex; i++)
		cout << this->indexAnsBfs[i] << ' ';
	cout << endl;

}