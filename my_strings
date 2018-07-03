//1.实现strcpy 
//2.实现strcat 
//3.实现strstr 
//4.实现strchr 
//5.实现strcmp 
//6.实现memcpy 
//7.实现memmove
#include<stdio.h>
#include<assert.h>
//strcpy
char *my_strcpy(char *dst,const char *src)
{
    if(dst == NULL || src == NULL)
        return NULL;
     const char *s = src;
    char *d = dst;
    while(*s != '\0')
    {
        *d = *s;
        d++;s++;
    }
    *d = '\0';
    return dst;
}



//strcat
char *my_strcat(char *dst,const char *src)
{
    if(dst == NULL || src == NULL)
        return NULL;
    char *d = dst;
    const char *s = src;
    while(*d != '\0')
    {
        d++;
    }
    while(*s != '\0')
    {
        *d++ = *s++;
    }
    *d = '\0';
    return dst;
}
//3.实现strstr 
const char *my_strstr(const char *dst, const char *src)
{
    assert(dst);
    assert(src);
    const char *s = src;
    const char *d = dst;
    while(*d != '\0')
    {
        const char *ret;
        ret = d;
        s = src;
        while(*s != '\0' && *s == *d)
        {
            s++;
            d++;
        }
        if(*s == '\0')
            return ret;
        d++;
    }
    return NULL;
}
//4.实现strchr 
const char *my_strchr(const char *dst, int c)
{
    if(NULL == dst)
        return NULL;
    const char *d = dst;
    while(*d != '\0')
    {
        if(*d == c)
            return d;
        else 
            d++;
    }
    return NULL;
}
//5.实现strcmp 
int my_strcmp(const char *dst,const char *src)
{
    const char *d = dst;
    const char *s = src;
    while(*s != '\0' && *d != '\0')
    {
        if(*s > *d)
            return -1;
        else if(*d < *s)
            return 1;
        else if(*d == *s)
        {
            d++;
            s++;
        }
    }
    if(*s == '\0'&& *d == '\0')
        return 0;
    else if(*d == '\0')
        return -1;
    else
        return 1;
}
//6.实现memcpy 
void *my_memcpy(void *dst, const void *src,size_t size)
{
    if(dst == NULL || src == NULL)
        return NULL;
    char *d =(char *) dst;
    char *s =(char *) src;
    while(size --)
    {
        *d++ = *s++;
    }
    return dst;
}
//7.实现memmove
void *my_memmove(void *dst,const void *src,size_t size)
{
    if(NULL == dst || NULL == src)
        return NULL;
    char *d = (char *)dst;
    const char *s =(char *)src;
    int i = 0;
    if(d < s)
    {
        for(; i < (int)size;i++)
        {
            *d++ = *s++;
        }
    }
    else if(d >= s)
    {
        for(i = (int)size - 1; i >= 0; i--)
        {
            *(d + i) = *(s + i);
        }
    }
    return dst;
}
int main()
{
   char dst[80]= "12345678901234567890";
   const char *src = "world!";
//    printf("%s               %s\n",strcpy(dst,src),my_strcpy(dst,src));
//    char dst1[80]="hello ";
//    const char *src1 = "world!";
//    printf("%s\n",strcat(dst1,src1));
//    printf("%s\n",my_strcat(dst1,src1));
//   printf("%s \n", my_strstr(dst,src)); 
//    printf("%s \n",my_strchr(dst,'c'));
//    printf("%d\n",my_strcmp(dst,src));
//    printf("%s\n",my_memcpy(dst,src,5));
    printf("%s \n",my_memmove(dst,dst+8,5));
    return 0;
}
