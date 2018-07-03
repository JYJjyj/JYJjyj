#include "DLinkList.h"

//新建一个结点
DLinkNode* CreateNode(DLinkType value)
{
    DLinkNode *ret = (DLinkNode*)malloc(sizeof(DLinkNode));
    ret->data = value;
    ret->prev = NULL;
    ret->next = NULL;
    return ret;
}
//销毁一个结点
void DestroyNode(DLinkNode *node)
{
    free(node);
    node = NULL;
}
void DLinkListInit(DLinkList** l)
{
    assert(l);
    (*l) = (DLinkList*)malloc(sizeof(DLinkNode));
    (*l)->head = CreateNode('x');
    (*l)->tail = (*l)->head;
}
DLinkNode* DLinkListPushBack(DLinkList* l, DLinkType value)
{
    assert(l);
    DLinkNode *newnode = CreateNode(value);
    l->tail->next = newnode;
    newnode->prev = l->tail;
    l->tail = newnode;
    return l->tail;
}

void DLinListPopBack(DLinkList* l)
{
    DLinkNode *tmp = l->tail->prev;
    tmp->next = NULL;
    DestroyNode(l->tail);
    l->tail = tmp;
}

//
void DLinkListPushFront(DLinkList* l, DLinkType value)
{
    assert(l);
    DLinkNode *newnode = CreateNode(value);
    newnode->prev = l->head;
    newnode->next =l->head->next;
    if(l->head->next == NULL)
    {
        l->head->next = newnode;
        l->tail = newnode;
        return;
    }
    l->head->next = newnode;
    newnode->next->prev = newnode;
}
void DLinkListPopFront(DLinkList* l)
{
    assert(l);
    if(l->head == NULL)
        return;
    DLinkNode *tmp = l->head->next;
    l->head->next = tmp->next;
    DestroyNode(tmp);
    if(l->head->next == NULL)
    {
        l->tail = l->head;
        return;
    }
    l->head->next->prev = l->head;
}
//
DLinkNode* DLinkListFind(DLinkList* l, DLinkType to_find)
{
    assert(l);
    DLinkNode *tmp = l->head->next;
    while(tmp != NULL)
    {
        if(tmp->data == to_find)
            return tmp;
        tmp = tmp->next;
    }
    return NULL;
}
//
////往指定位置之前插入一个元素 
void DLinkListInsert(DLinkNode* pos, DLinkType value)
{
    assert(pos);
    DLinkNode *newnode = CreateNode(value);
    newnode->next = pos;
    newnode->prev = pos->prev;
    newnode->prev->next = newnode;
    pos->prev = newnode;
}
//
////往指定位置之后插入一个元素 
void DLinkListInsertAfter(DLinkList *l,DLinkNode* pos, DLinkType value)
{
    assert(pos);
    DLinkNode *newnode = CreateNode(value);
    if(pos->next == NULL)
    {
        newnode->prev = pos;
        pos->next = newnode;
        l->tail = newnode;
        return ;
    }
    newnode->prev = pos;
    newnode->next = pos->next;
    pos->next = newnode;
    newnode->next->prev = newnode;
}

void DLinkListErase(DLinkList *l)
{
    assert(l);
    DLinkNode *tmp = l->head->next;
    while(tmp != NULL)
    {
        DLinkNode *next = tmp->next;
        DestroyNode(tmp);
        tmp = next;
    }
    l->tail = l->head;
    l->head->next = NULL;
}
//
void DLinkListRemove(DLinkList *l,DLinkType value)
{
    assert(l);
    DLinkNode *tmp = l->head->next;
    while(tmp != NULL)
    {
        if(tmp->data == value)
        {
            if(tmp == l->tail)
            {
                l->tail = tmp->prev;
                tmp->prev->next = NULL;
                return;
            }
            DLinkNode *next = tmp->next;
            next->prev = tmp->prev;
            tmp->prev->next = next;
            DestroyNode(tmp); 
            return;
        }
        else
            tmp = tmp->next;
    }
    return;
}
//
void DLinkListRemoveAll(DLinkList *l,DLinkType value)
{
    assert(l);
    DLinkNode *tmp = l->head->next;
    while(tmp != NULL)
    {
        if(tmp->data == value)
        {
            if(tmp == l->tail)
            {
                l->tail = tmp->prev;
                l->tail->next = NULL;
                DestroyNode(tmp);
                return;
            }
            DLinkNode *next = tmp->next;
            tmp->prev->next = next;
            next->prev = tmp->prev;
            DestroyNode(tmp);
            tmp = next;
        }
        else
            tmp = tmp->next;
    }
}
//
size_t DLinkListSize(DLinkList *l)
{
    DLinkNode *tmp = l->head;
    size_t count = 0;
    while(tmp != l->tail->next)
    {
        count++;
        tmp = tmp->next;
    }
    return count;
}

//参数非法返回 -1 ，空链表返回 1 ，非空链表返回0。
int DLinkListEmpty(DLinkList *l)
{
    if(l == NULL)
        return -1;
    else if(l->head == NULL)
        return -1;
    else if(l->head->next == NULL)
        return 1;
    else
        return 0;
}

void Print(DLinkList *l,const char *msg)
{
    printf("%s:",msg);
    DLinkNode *tmp = l->head->next;
    printf("[head] ");
    while(tmp!=l->tail->next)
    {  
        printf("[%c] ",tmp->data);
        tmp = tmp->next;
    }
    printf("\n");
}
///////////////////////////////////测试代码///////////////////////////
#define TESTHEAD printf("-----------------------------%s--------------------------------\n",__FUNCTION__)
void testPushBack()
{
    TESTHEAD;
    DLinkList *l;
    DLinkListInit(&l);
    DLinkListPushBack(l,'a');
    DLinkListPushBack(l,'b');
    DLinkListPushBack(l,'c');
    DLinkListPushBack(l,'d');
    DLinkListPushBack(l,'e');
    DLinkListPushBack(l,'f');
    Print(l,"尾插六个元素");
}
void testPopBack()
{
    TESTHEAD;
    DLinkList *l;
    DLinkListInit(&l);
    DLinkListPushBack(l,'a');
    DLinkListPushBack(l,'b');
    DLinkListPushBack(l,'c');
    DLinkListPushBack(l,'d');
    DLinkListPushBack(l,'e');
    DLinkListPushBack(l,'f');
    Print(l,"尾插后");
    DLinListPopBack(l);
    Print(l,"尾删一个元素后");
}
void testPushFront()
{
    TESTHEAD;
    DLinkList *l;
    DLinkListInit(&l);
    DLinkListPushFront(l,'a');
    DLinkListPushFront(l,'b');
    DLinkListPushFront(l,'c');
    DLinkListPushFront(l,'d');
    DLinkListPushFront(l,'e');
    DLinkListPushFront(l,'f');
    Print(l,"头插后");

}
void testPopFront()
{
    TESTHEAD;
    DLinkList *l;
    DLinkListInit(&l);
    DLinkListPushFront(l,'a');
    DLinkListPushFront(l,'b');
    DLinkListPushFront(l,'c');
    DLinkListPushFront(l,'d');
    DLinkListPushFront(l,'e');
    DLinkListPushFront(l,'f');
    Print(l,"头插后");
    DLinkListPopFront(l);
    DLinkListPopFront(l);
    DLinkListPopFront(l);
    DLinkListPopFront(l);
    DLinkListPopFront(l);
    DLinkListPopFront(l);
    Print(l,"头删：");
}
void testListFind()
{
    TESTHEAD;
    DLinkList *l;
    DLinkListInit(&l);
    DLinkListPushFront(l,'a');
    DLinkListPushFront(l,'b');
    DLinkListPushFront(l,'c');
    DLinkListPushFront(l,'d');
    DLinkListPushFront(l,'e');
    DLinkListPushFront(l,'f');
    Print(l,"头插后");
    DLinkNode *ret = DLinkListFind(l,'x');
    if(ret == NULL)
    {
        printf("没找到！\n");
    }
    else if(ret->data == 'x')
        printf("找到了！\n");
}
void testRemove()
{
    TESTHEAD;
    DLinkList *l;
    DLinkListInit(&l);
    DLinkListPushFront(l,'a');
    DLinkListPushFront(l,'b');
    DLinkListPushFront(l,'c');
    DLinkListPushFront(l,'d');
    DLinkListPushFront(l,'e');
    DLinkListPushFront(l,'f');
    Print(l,"头插后");
    DLinkListRemove(l,'a');
    Print(l,"删除一个f后");
}
void testErase()
{
    TESTHEAD;
    DLinkList *l;
    DLinkListInit(&l);
    DLinkListPushFront(l,'a');
    DLinkListPushFront(l,'b');
    DLinkListPushFront(l,'c');
    DLinkListPushFront(l,'d');
    DLinkListPushFront(l,'e');
    DLinkListPushFront(l,'f');
    Print(l,"头插后");
    DLinkListErase(l);
    Print(l,"清空链表后");
}
void testEmpty()
{
    TESTHEAD;
    DLinkList *l;
    DLinkListInit(&l);
    DLinkListPushFront(l,'a');
    DLinkListPushFront(l,'b');
    DLinkListPushFront(l,'c');
    DLinkListPushFront(l,'d');
    DLinkListPushFront(l,'e');
    DLinkListPushFront(l,'f');
    Print(l,"头插后");
    int ret = DLinkListEmpty(l);
    printf("expect 0 actual：%d\n",ret);
    DLinkListErase(l);
    ret = DLinkListEmpty(l);
    printf("expect 1 actual: %d\n",ret);
}
void testRemoveAll()
{
    TESTHEAD;
    DLinkList *l;
    DLinkListInit(&l);
    DLinkListPushFront(l,'a');
    DLinkListPushFront(l,'a');
    DLinkListPushFront(l,'c');
    DLinkListPushFront(l,'a');
    DLinkListPushFront(l,'e');
    DLinkListPushFront(l,'a');
    Print(l,"头插后");
    DLinkListRemoveAll(l,'a');
    Print(l,"删除所以的a");
}
void testInsert()
{
    TESTHEAD;
    DLinkList *l;
    DLinkListInit(&l);
    DLinkNode* pos = DLinkListPushBack(l,'a');
    DLinkListPushBack(l,'b');
    DLinkListPushBack(l,'c');
    DLinkListPushBack(l,'d');
    DLinkListPushBack(l,'e');
    DLinkListPushBack(l,'f');
    Print(l,"尾插后");
    DLinkListInsert(pos,'x');
    Print(l,"在a之前插入一个x");
}
void testInsertAfter()
{
    TESTHEAD;
    DLinkList *l;
    DLinkListInit(&l);
    DLinkListPushBack(l,'a');
    DLinkListPushBack(l,'b');
    DLinkListPushBack(l,'c');
    DLinkListPushBack(l,'d');
    DLinkListPushBack(l,'e');
    DLinkNode* pos = DLinkListPushBack(l,'f');
    Print(l,"尾插后");
    DLinkListInsertAfter(l,pos,'x');
    Print(l,"在a之后插入一个x");
}
void test()
{
    testPushBack();
    testPopBack();
    testPushFront();
    testPopFront();
    testListFind();
    testRemove();
    testErase();
    testEmpty();
    testRemoveAll();
    testInsert();
    testInsertAfter();
}
int main()
{
    test();
    return 0;
}
