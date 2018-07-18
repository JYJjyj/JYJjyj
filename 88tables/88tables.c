#include <stdio.h>
#include <stdlib.h>

int main()
{
    for(int i = 1; i <= 8; i++)
    {
        printf("\033[3%dm",i - 1);
        printf("\033[4%dm",(i + 1) % 8);
        for(int j = 1; j<= 8; j++)
        {
            if(j <= i)
                printf("%dx%d=%-3d",j, i, j * i);
        }
        printf("\n");
    }
    printf("\033[0m");
}
