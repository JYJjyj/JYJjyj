//栈与队列的面试题
//1.最小栈(MinStack)
//2.两个栈实现一个队列
//3.两个队列实现一个栈
//4.判定字符串是否按照出栈顺序
//5.共享栈(一个数组实现两个栈)

//2
#include"SeqStack.c"
#include<stdio.h>
#include<stddef.h>
#include<assert.h>
void Two_Stack_Become_A_Queue()
{
  SeqStack s1;
  SeqStack s2;
  SeqStackInit(&s1);
  SeqStackInit(&s2);
  DataType arr[] = "fedcba";
  size_t size = sizeof(arr)/sizeof(DataType);
  //将元素先全部入栈到第一个栈中
  size_t i;
  for(i = 0; i < size; i++)
  {
    SeqStackPush(&s1,arr[i]);
  }
  Print(&s1);
  //取栈顶元素，并将其出栈，再入栈到第二个栈中。
  for(i = 0; i < size; i++)
  {
    DataType tmp = SeqStackTop(s1);
    SeqStackPop(&s1);
    SeqStackPush(&s2,tmp);
  }
  //打印当前第二个栈，就得到了给定的数组实现栈的功能
  Print(&s2);
}

int main()
{
  Two_Stack_Become_A_Queue();
  return 0;
}

