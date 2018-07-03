#include <stdio.h>
char* my_strcpy(char *DesStr , const char *SrcStr)
{
    char *ret=DesStr;
    if(DesStr == NULL || SrcStr == NULL)
        return NULL;
    while(*SrcStr != '\0')
    {
       *(DesStr++) = *(SrcStr++);
    }
    *DesStr='\0';
    return ret;
}
int main()
{
    char des[80];
    const char *src = "world";
    char *ret = my_strcpy(des,src);
    printf("%s \n",ret);
}
