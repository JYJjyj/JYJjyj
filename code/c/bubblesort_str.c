#include <stdio.h>
#include <string.h>
void BubbleStr(char *str[],int num)
{
    if(num == 0 || num == 1)
      return;
    int i=0,j=0;
    for(;i<num-1;i++)
    {
        for(;j<num-i-1;j++)
        { 
            if (strcmp(str[j],str[j+1])>0)
            {
                char *tmp;
                tmp = str[j];
                str[j] = str[j+1];
                str[j+1] = tmp;
            }
        }    
    }
}

int main()
{
    char *str[] = {"bcdet","abcd","b"};
    BubbleStr(str,3);
    int i=0;
    for(;i<3;i++)
    {
        printf("%s   ",str[i]);
    }
    printf("\n");
    return 0;
}
