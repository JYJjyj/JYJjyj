#include <stdio.h>
char* my_strcat(char *DstStr, const char *SrcStr)
{
    if(DstStr == NULL || SrcStr == NULL)
        return NULL;
    char *dst = DstStr;
    const char *src = SrcStr;
    while(*dst != '\0')
    {
        dst++;
    }
    while(*src != '\0')
    {
        *dst++ =*src++;
    }
    *dst='\0';
    return DstStr;
}

int main()
{
    char dst[80]="hello ";
    const char *src = "world";
    char *ret;
    ret = my_strcat(dst,src);
    printf("%s\n",ret);
    return 0;
}
