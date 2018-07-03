#pragma once
//顺序栈的实现（c语言）
#include<stdio.h>
#include<stddef.h>
#include<assert.h>
#include<malloc.h>
#include<memory.h>
#define MAXSIZE 100
typedef char DataType;
typedef struct SeqStack
{
    DataType *data;
    size_t size;
    size_t capacity;
}SeqStack;
//初始化 
void SeqStackInit(SeqStack *s);
//销毁 
void SeqStackDestroy(SeqStack *s);
//入栈 
void SeqStackPush(SeqStack *s,DataType data);
//出栈 
void SeqStackPop(SeqStack *s);
//取栈顶元素 
DataType SeqStackTop(SeqStack s);
