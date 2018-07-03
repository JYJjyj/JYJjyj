//实现strncpy  strncat strncmp
#include<stdio.h>
#include<assert.h>
//strncpy
char *my_strncpy(char *dst,const char *src,size_t n)
{
    assert(dst);
    assert(src);
    size_t  i = 0;
    char *d = dst;
    const char *s = src;
    for(;i<n;i++)
    {
        if(*s == '\0')
        {
            break;
        }
        *d++ = *s++;
    }
    return dst;
}
//strncat
char *my_strncat(char *dst,const char *src,size_t n)
{
    assert(dst);
    assert(src);
    size_t i = 0;
    char *d = dst;
    const char *s = src;
    while(*d != '\0')
    {
        d++;
    }
    for(;i<n;i++)
    {
        if(*s == '\0')
            break;
        else
        {
            *d++ = *s++;
        }
    }
    return dst;
}
//strncmp
int my_strncmp(char *dst,const char *src,size_t n)
{
    assert(dst);
    assert(src);
    char *d = dst;
    const char *s = src;
    size_t i = 0;
    for(;i<n;i++)
    {
        if(*s == '\0')
            break;
        else if(*d > *s)
            return 1;
        else if(*s > *d)
            return -1;
        else if(*s == *d)
        {
            s++;
            d++;
        }
    }
    return 0;
}
int main()
{
    char dst[80] = "abcdefgh";
    const char *src = "abcdefghio";
//    printf("%s\n",my_strncpy(dst,src,8));
//    printf("%s\n",my_strncat(dst,src,10));
    printf("%d\n",my_strncmp(dst,src,8));
    return 0;
}
