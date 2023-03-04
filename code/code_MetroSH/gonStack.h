//@time   : 2022.8.31
//@author : gonzalez
//@func   : 自定义栈
//@notice : 使用类模板-->定义和声明需要均写在头文件h中

#pragma once
#include<iostream>

/***************************************** gonStack声明 *****************************************/
//@name   : gonStack栈类
//@func   : 自行实现栈
//@time   : 2022.8.31
template<typename TYPE>
struct stackNode {
	TYPE value;
	stackNode* next = NULL;
};
template<typename TYPE>
class gonStack {
protected:
	stackNode<TYPE>* stackTop = NULL;
	stackNode<TYPE>* stackHead = NULL;
	int length = 0;
public:
	gonStack();                        // 栈构造函数
	~gonStack();                       // 栈析构函数
	bool empty(void);                  // 判断栈空
	TYPE top(void);                    // 求出栈顶元素
	bool pop(void);                    // 弹出栈顶元素
	bool push(TYPE tempValue);         // 元素入栈
	int size(void);                    // 求出栈长度
}; 

/***************************************** gonStack定义 *****************************************/
//@name : gonStack
//@func : gonStack类构造函数
//@time : 2022.8.31
template<typename TYPE>
gonStack<TYPE>::gonStack()
{
	this->stackHead = new stackNode<TYPE>;
	this->length = 0;
	this->stackTop = this->stackHead;
}
//@name : empty
//@func : 判断栈是否为空(ture为空 | false不为空)
//@time : 2022.8.31
template<typename TYPE>
bool gonStack<TYPE>::empty(void)
{
	if (this->length == 0 && this->stackHead == this->stackTop)
		return true;
	else
		return false;
}
//@name   : top
//@func   : 返回栈顶部的值
//@time   : 2022.8.31
//@notice : 栈空将返回报错！！！
//          需要进行empty判断！！！
template<typename TYPE>
TYPE gonStack<TYPE>::top(void)
{
	return this->stackTop->value; // 正常返回
}
//@name : pop
//@func : 栈弹出节点
//@time : 2022.8.31
template<typename TYPE>
bool gonStack<TYPE>::pop(void)
{
	if (!this->empty()) {
		stackNode<TYPE>* temp = this->stackHead;
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
//@time : 2022.8.31
template<typename TYPE>
bool gonStack<TYPE>::push(TYPE tempValue)
{
	stackNode<TYPE>* temp = new stackNode<TYPE>;   //头结点更新
	this->stackTop->next = temp;                   //头结点更新
	this->stackTop = temp;                         //头指针更新
	this->length++;                                //栈长度更新
	this->stackTop->value = tempValue;
	return true;
}
//@name : size
//@func : 求得栈长度
//@time : 2022.8.31
template<typename TYPE>
int gonStack<TYPE>::size(void)
{
	return this->length;
}
//@name : gonStack
//@func : gonStack类析构函数
//@time : 2022.8.31
template<typename TYPE>
gonStack<TYPE>::~gonStack()
{
	while (this->stackHead != this->stackTop) {
		stackNode<TYPE>* temp = this->stackHead->next;
		delete this->stackHead;
		this->stackHead = temp;
	}
	delete this->stackHead;
	this->stackHead = NULL;
	this->stackTop = NULL;
}
