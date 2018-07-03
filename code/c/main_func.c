//n函数的参数，实现一个整数计算器，程序可以接受三个参数，
//第一个参数“-a”选项执行加法，“-s”选项执行减法，“-m”选项执行乘法，“-d”选项执行除法，后面两个参数为操作数。 
//例如：命令行参数输入：test.exe -a 1 2 
//执行1+2输出3 使用main函数的参数，实现一个整数计算器，程序可以接受三个参数，
//第一个参数“-a”选项执行加法，“-s”选项执行减法，“-m”选项执行乘法，“-d”选项执行除法，后面两个参数为操作数。 
//例如：命令行参数输入：test.exe -a 1 2
//执行1+2输出3 使用main函数的参数，实现一个整数计算器，程序可以接受三个参数，
//第一个参数“-a”选项执行加法，“-s”选项执行减法，“-m”选项执行乘法，“-d”选项执行除法，后面两个参数为操作数。 
//例如：命令行参数输入：test.exe -a 1 2 
//执行1+2输出3 


#include<stdio.h>
#include<assert.h>
#include<string.h>
int add(int x,int y)
{
    return x + y;
}

int sub(int x,int y)
{
    return x - y;
}

int mul(int x,int y)
{
    return x * y;
}

int div(int x,int y)
{
    assert(y == 0);
    return x / y;
}

int main(int argc,char *argv[])
{
    int x,y;
    scanf("%d %d",&x,&y);
    if(argc != 2)
        return 0;
    if( strcmp(argv[1],"-a") == 0)
        printf("x + y = %d\n",add(x,y));
    else if( strcmp(argv[1],"-s") == 0)
        printf("x - y = %d\n",sub(x,y));
    else if( strcmp(argv[1],"-m") == 0)
        printf("x * y = %d\n",mul(x,y));
    else if( strcmp(argv[1],"-d") == 0)
        printf("x / y = %d\n",div(x,y));
    return 0;
}
