#include "SeqStack.h"

//初始化 
void SeqStackInit(SeqStack *s)
{
    assert(s);
    s->size = 0;
    s->capacity = 3;
    s->data = (DataType*)calloc(3,sizeof(DataType)*3);
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
    memmove(tmp,s->data,s->size+1);
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
    s->data[s->size] = data;
    (s->size)++;
}
//出栈 
void SeqStackPop(SeqStack *s)
{
    assert(s);
    if(s->data == NULL)
        return;
    (s->size)--;
}
//取栈顶元素 
int SeqStackTop(SeqStack* s,DataType *value)
{
    assert(s);
    if(s->size > 0)
    {
        *value = s->data[s->size-1];
        return 1;
    }
    else
    {
        return 0;
    }
}
//打印顺序栈
void Print(SeqStack *s)
{
    DataType *tmp = s->data;
    while(*tmp != 0)
    {
        printf("%c ",*(tmp++));
    }
    printf("\n");
}
///////////////////////////////测试代码////////////////////////////////////
//#define TESTHEAD printf("----------------------------%s------------------------------\n",__FUNCTION__)
//
//void testPush()
//{
//   SeqStack s;
//   SeqStackInit(&s);
//   SeqStackPush(&s,'a');
//   SeqStackPush(&s,'b');
//   SeqStackPush(&s,'c');
//   SeqStackPush(&s,'d');
//   SeqStackPush(&s,'e');
//   Print(&s);
//}
//void testPop()
//{
//   SeqStack s;
//   SeqStackInit(&s);
//   SeqStackPush(&s,'a');
//   SeqStackPush(&s,'b');
//   SeqStackPush(&s,'c');
//   SeqStackPush(&s,'d');
//   SeqStackPush(&s,'e');
//   Print(&s);
//   SeqStackPop(&s);
//   Print(&s);
//   SeqStackPop(&s);
//   Print(&s);
//   SeqStackPop(&s);
//   Print(&s);
//   SeqStackPop(&s);
//   Print(&s);
//   SeqStackPop(&s);
//   Print(&s);
//}
//void testTop()
//{
//   SeqStack s;
//   SeqStackInit(&s);
//   SeqStackPush(&s,'a');
//   SeqStackPush(&s,'b');
//   SeqStackPush(&s,'c');
//   SeqStackPush(&s,'d');
//   SeqStackPush(&s,'e');
//   Print(&s);
//   DataType value;
//   if(SeqStackTop(&s,&value) == 0)
//       printf("seqstack empty!\n");
//   else{
//       printf("%c \n",value);
//   }
//}
//void testDestroy()
//{
//   SeqStack s;
//   SeqStackInit(&s);
//   SeqStackPush(&s,'a');
//   SeqStackPush(&s,'b');
//   SeqStackPush(&s,'c');
//   SeqStackPush(&s,'d');
//   SeqStackPush(&s,'e');
//   Print(&s);
//   SeqStackDestroy(&s);
//   printf("expect NULL actual %p\n",s.data);
//}
//void test()
//{
//    testPush();
//    testPop();
//    testTop();
//    testDestroy();
//}
//
//int main()
//{
//    test();
//    return 0;
//}
//
