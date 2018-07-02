#include "SeqStack.h"

//初始化 
void SeqStackInit(SeqStack *s)
{
    assert(s);
    s->size = 0;
    s->capacity = 3;
    s->data = (DataType*)malloc(sizeof(DataType)*3);
}
//销毁 
void SeqStackDestroy(SeqStack *s)
{
    assert(s);
    if(s->data == NULL)
        return;
    s->size = 0;
    s->capacity = 0;
    free(s->data);
    s->data = NULL;
    s = NULL;
}
//
//扩容
SeqStack* Expand(SeqStack *s)
{
    DataType *tmp  = (DataType*)malloc(2*s->capacity);
    memmove(tmp,s->data,s->size);
    free(s->data);
    s->data = tmp;
    s->capacity = (s->capacity) * 2;
    return s;
}
//入栈 
void SeqStackPush(SeqStack *s,DataType data)
{
    assert(s);
    if(s->data == NULL)
        return;
    if(s->size >= s->capacity)
        s = Expand(s);
    memmove(s->data + 1,s->data,s->size);
    s->size++;
    s->data[0] = data;
}
//出栈 
void SeqStackPop(SeqStack *s)
{
    assert(s);
    if(s->data == NULL)
        return;
    memmove(s->data,s->data + 1,s->size-1);
    s->size--;
}
//取栈顶元素 
DataType SeqStackTop(SeqStack s)
{
    assert(s.data);
    return *(s.data);
}
//打印顺序栈
void Print(SeqStack *s)
{
    size_t size = 1;
    DataType *tmp = s->data;
    while(size <= s->size)
    {
        printf("%c ",*tmp++);
        size++;
    }
    printf("\n");
}
/////////////////////////////////测试代码////////////////////////////////////
#define TESTHEAD printf("----------------------------%s------------------------------\n",__FUNCTION__)

void testPush()
{
   SeqStack s;
   SeqStackInit(&s);
   SeqStackPush(&s,'a');
   SeqStackPush(&s,'b');
   SeqStackPush(&s,'c');
   SeqStackPush(&s,'d');
   SeqStackPush(&s,'e');
   Print(&s);
}
void testPop()
{
   SeqStack s;
   SeqStackInit(&s);
   SeqStackPush(&s,'a');
   SeqStackPush(&s,'b');
   SeqStackPush(&s,'c');
   SeqStackPush(&s,'d');
   SeqStackPush(&s,'e');
   Print(&s);
   SeqStackPop(&s);
   Print(&s);
   SeqStackPop(&s);
   Print(&s);
   SeqStackPop(&s);
   Print(&s);
   SeqStackPop(&s);
   Print(&s);
   SeqStackPop(&s);
   Print(&s);
}
void testTop()
{
   SeqStack s;
   SeqStackInit(&s);
   SeqStackPush(&s,'a');
   SeqStackPush(&s,'b');
   SeqStackPush(&s,'c');
   SeqStackPush(&s,'d');
   SeqStackPush(&s,'e');
   Print(&s);
   printf("栈顶元素：%c \n",SeqStackTop(s));
}
void testDestroy()
{
   SeqStack s;
   SeqStackInit(&s);
   SeqStackPush(&s,'a');
   SeqStackPush(&s,'b');
   SeqStackPush(&s,'c');
   SeqStackPush(&s,'d');
   SeqStackPush(&s,'e');
   Print(&s);
   SeqStackDestroy(&s);
   printf("expect NULL actual %p\n",s.data);
}
void test()
{
    testPush();
    testPop();
    testTop();
    testDestroy();
}

int main()
{
    test();
    return 0;
}
