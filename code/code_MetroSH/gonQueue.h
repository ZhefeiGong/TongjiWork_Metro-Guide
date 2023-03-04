//@time   : 2022.8.31
//@author : gonzalez
//@func   : 自定义队列
//@notice : 使用类模板-->定义和声明需要均写在头文件h中

#pragma once
#include<iostream>

/***************************************** gonQueue声明 *****************************************/
//@name : gonQueue队列类
//@func : 自行实现队列
//@time : 2022.8.31
template<typename TYPE>
struct queueNode {
	queueNode* before = NULL;
	TYPE value;
	queueNode* after = NULL;
};
template<typename TYPE>
class gonQueue {
protected:
	queueNode<TYPE>* queueHead = NULL;
	queueNode<TYPE>* queueTail = NULL;
	int length = 0;
public:
	gonQueue();                    // 队列构造函数
	~gonQueue();                   // 队列析构函数
	bool empty(void);              // 判断队列空
	TYPE front(void);              // 得到队列首元素
	TYPE back(void);               // 得到队列尾元素
	bool pop(void);                // 弹出队列头元素
	bool push(TYPE tempValue);     // 队列尾入队元素
	int size(void);                // 得到队列长度
};

/***************************************** gonQueue定义 *****************************************/
//@name : gonQueue
//@func : gonQueue类构造函数
//@time : 2022.8.31
template<typename TYPE>
gonQueue<TYPE>::gonQueue()
{
	this->length = 0;
}
//@name : empty
//@func : 判断队列是否为空
//@time : 2022.8.31
template<typename TYPE>
bool gonQueue<TYPE>::empty(void)
{
	if (this->length == 0 && this->queueHead == NULL && this->queueTail == NULL)
		return true;
	else
		return false;
}
//@name   : front
//@func   : 返回队列头部值
//@time   : 2022.8.31
//@notice : 需人为保证队列非空 -- 否则报错
template<typename TYPE>
TYPE gonQueue<TYPE>::front(void)
{
	return this->queueHead->value;
}
//@name   : back
//@func   : 返回队列尾部值
//@time   : 2022.8.31
//@notice : 需人为保证队列非空 -- 否则报错
template<typename TYPE>
TYPE gonQueue<TYPE>::back(void)
{
	return this->queueTail->value;
}
//@name : pop
//@func : 队列头部弹出
//@time : 2022.8.31
template<typename TYPE>
bool gonQueue<TYPE>::pop(void)
{
	if (!this->empty()) {
		queueNode<TYPE>* temp = this->queueHead->after;
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
	else {
		return false;
	}
}
//@name : push
//@func : 队列尾部增添
//@time : 2022.8.31
template<typename TYPE>
bool gonQueue<TYPE>::push(TYPE tempValue)
{
	// 队列为空
	if (this->empty()) {
		this->queueHead = new queueNode<TYPE>;
		this->queueTail = this->queueHead;
		this->queueTail->value = tempValue;
	}
	// 队列尾部添加
	else {
		queueNode<TYPE>* temp = new queueNode<TYPE>;
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
//@time : 2022.8.31
template<typename TYPE>
int gonQueue<TYPE>::size(void)
{
	return this->length;
}
//@name : ~gonQueue
//@func : gonQueue类析构函数
//@time : 2022.8.31
template<typename TYPE>
gonQueue<TYPE>::~gonQueue()
{
	if (!this->empty()) {
		while (this->queueHead != this->queueTail) {
			queueNode<TYPE>* temp = this->queueTail->before;
			delete this->queueTail;
			this->queueTail = temp;
		}
		delete this->queueHead;
		this->queueHead = NULL;
		this->queueTail = NULL;
	}
}



