#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <stdlib.h>
typedef struct node
{
    char* name[1024];
    node *_next;
    node *_prev;
}node;
//插入歌曲
void list_insert(char *name)
{

}

void list_show()
{

}

void mus_read()
{

}
//音乐菜单
int menu()
{
    printf("==================我的播放器=======================\n");
    list_show();
    printf("---------------------------------------------------\n");
    printf("\t1.播放/暂停\n");
    printf("\t2.下一首\n");
    printf("\t3.上一首\n");
    printf("\t4.停止播放\n");
    printf("\t0.退出\n");
    printf("> ");
    int choose;
    scanf("%d",&choose);
    return choose;
}
void playPause()
{

}
void stop(){}
void next(){}
void prev(){}
int main()
{
    int choose;
    mus_read();
    do{
        choose = menu();
        switch(choose){
            case 1:
                playPause();
                break;
            case 2:
                next();
                break;
            case 3:
                prev();
                break;
            case 4:
                stop();
                break;
            case 0:
                system("killall -SIGLILL aplay");
        }
    }while(choose != 0); 
    list_show();
    return 0;
}
