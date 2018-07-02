#include <stdio.h>
#include <stddef.h>
void find(int arr[],size_t length,int *num1,int *num2)
{
    if(NULL == arr)
        return;
    //先将所有数字进行异或，得到的值就是这两个只出现一次的数相异或的数字
    //得到按照任意一位不相同的二进制位对这个数组进行分组，分为两组，每一组中包含一个只出现了一次的数
    //组内进行异或，得到该组中只出现了一次的那个数字
    size_t i=1;
    int ret = arr[0];
    for(;i < length; i++)
    {
        ret = ret ^ arr[i];
    }
    size_t  num = 0;
    while(1)
    {
        if((ret & 1) == 1)
            break;
        else
        {
            num++;
            ret >>= 1;
        }
    }
    *num1 = *num2 = 0;
    for(i = 0; i<length; i++)
    {
        int tmp = arr[i];
        if(((tmp>>num) & 1) == 1)
            *num1 = arr[i] ^ *num1;
        else
            *num2 = arr[i] ^ *num2;
    }
}

int main()
{
    int arr[10]={1,5,8,0,4,0,5,4,2,2};
    int num1,num2;
    find(arr,sizeof(arr)/sizeof(int),&num1,&num2);
    printf("num1 = %d\n num2 = %d\n",num1,num2);
    return 0;
}
