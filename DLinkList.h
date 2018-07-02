//带有头指针与尾指针的双向链表(有头指针)
#pragma once
#include<stdio.h>
#include<assert.h>
#include<malloc.h>
#include<stddef.h>
typedef char DLinkType; 

typedef struct DLinkNode { 
    DLinkType data; 
    struct DLinkNode* next; 
    struct DLinkNode* prev; 
} DLinkNode; 
typedef struct DLinkList
{
    //头指针
    DLinkNode *head;
    //尾指针
    DLinkNode *tail;
}DLinkList;
//初始化链表
void DLinkListInit(DLinkList** l); 
//尾插一个结点
DLinkNode* DLinkListPushBack(DLinkList* l, DLinkType value); 
//尾删
void DLinListPopBack(DLinkList* l); 
//头插
void DLinkListPushFront(DLinkList* l, DLinkType value); 
//头删
void DLinkListPopFront(DLinkList* l); 
//查找
DLinkNode* DLinkListFind(DLinkList* l, DLinkType to_find); 

//往指定位置之前插入一个元素 
void DLinkListInsert(DLinkNode* pos, DLinkType value); 

//往指定位置之后插入一个元素 
void DLinkListInsertAfter(DLinkList *l,DLinkNode* pos, DLinkType value); 
//清空一个链表
void DLinkListErase(); 
//删除一个指定结点
void DLinkListRemove(DLinkList *l,DLinkType value); 
//删除所有在这个链表中的值为给定值的结点
void DLinkListRemoveAll(DLinkList *l,DLinkType value); 
//求一个链表的长度
size_t DLinkListSize(); 
//判断一个链表是否为空
int DLinkListEmpty();
