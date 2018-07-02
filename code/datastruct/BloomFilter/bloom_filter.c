#include <stdio.h>
#include"bloom_filter.h"
size_t BKDRHash(const char *str)  
{  
    size_t hash = 0;  
    size_t ch; 
    while (ch = (size_t)*str++)  
    {         
            hash = hash * 131 + ch;   // 也可以乘以31、131、1313、13131、131313..  
    }
    return hash;  
}
size_t SDBMHash(const char *str)  
{  
    size_t hash = 0;  
    size_t ch;
    while (ch = (size_t)*str++)  
    {  
            hash = 65599 * hash + ch;         
    }
    return hash;
}
void BloomFilterInit(BloomFilter* bf)
{
    if(NULL == bf)
        return;
    BitMapInit(&bf->bitmap,BitMapCapacity);
    bf->hash_func[0]=BKDRHash;
    bf->hash_func[1]=SDBMHash;
}


void BloomFilterInsert(BloomFilter* bf, const char* str)
{
    if(NULL == bf)
        return;
    size_t i = 0;
    for(;i<HashFuncMaxSize;i++)
    {
        size_t ret = bf->hash_func[i](str) % BitMapCapacity;
       //这里要记得对位图的最大值进行求模运算，哈希函数算出的只是哈希值并不是在位图中要设置的位数。
        BitMapSet(&bf->bitmap,ret);
    }
}


int BloomFilterIsExist(BloomFilter* bf, const char* str)
{
    if(NULL == bf)
        return 0;
    size_t i=0;
    for(;i<HashFuncMaxSize;i++)
    {
      size_t ret;  
      ret = bf->hash_func[i](str) % BitMapCapacity;
      size_t tmp;
      tmp = BitMapTest(&bf->bitmap,ret);
      if(tmp == 0)
          return 0;
    }
    return 1;
}

void BloomFilterDestroy(BloomFilter* bf)
{
    if(NULL == bf)
        return;
    BitMapDestroy(&bf->bitmap);
    size_t i=0;
    for(;i<HashFuncMaxSize;i++)
    {
        bf->hash_func[i] = NULL;
    }
}
///////////////////////////////////////////////////////测试代码//////////////////////////////////////////////////////////////

#define __TestHead__ printf("--------------------------------%s---------------------------------\n",__FUNCTION__)
void TestBloomFilterInit()
{
    __TestHead__;
    BloomFilter bf;
    BloomFilterInit(&bf);
    printf("bf.func1 expect %p ,  actual %p \n",bf.hash_func[0],BKDRHash);
    printf("bf.func2 expect %p ,  actual %p \n",bf.hash_func[1],SDBMHash);
}
void TestBloomFilterInsert()
{
    __TestHead__;
    BloomFilter bf;
    BloomFilterInit(&bf);
    size_t i;
    for(i=0;i<GetSize(bf.bitmap.capacity);i++)
    {
        printf("[%lu]:%lu",i,bf.bitmap.data[i]);
    }
    printf("\n");
    BloomFilterInsert(&bf,"hello world");
    for(i=0;i<GetSize(bf.bitmap.capacity);i++)
    {
        printf("[%lu]:%lu",i,bf.bitmap.data[i]);
    }
    printf("\n");
}
void TestBloomFilterExit()
{

    __TestHead__;
    BloomFilter bf;
    BloomFilterInit(&bf);
    BloomFilterInsert(&bf,"hello world");
    int ret;
    ret = BloomFilterIsExist(&bf,"hello world");
    printf("expect 1  actual %d\n",ret);
    ret = BloomFilterIsExist(&bf,"hello");
    printf("expect 0  actual %d\n",ret);
}
void TestBloomFilterDestroy()
{
    __TestHead__;
    BloomFilter bf;
    BloomFilterInit(&bf);
    size_t i=0;
    BloomFilterInsert(&bf,"hello");
    BloomFilterDestroy(&bf);
    for(;i<HashFuncMaxSize;i++)
    {
        if(bf.hash_func[i] != NULL)
            printf("destroy error!\n");
    }
    printf("destroy complete!\n");
}
void test()
{
    TestBloomFilterInit();
    TestBloomFilterInsert();
    TestBloomFilterExit();
    TestBloomFilterDestroy();
}

int main()
{
    test();
    return 0;
}





















