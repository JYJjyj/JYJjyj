#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <memory.h>
#include <sys/types.h>
#include <dirent.h>
// cp src dst
void cp_file(const char* src, const char* dst, mode_t mode)
{
    int fd_src = open(src,O_RDONLY,mode);
    if(fd_src < 0)
    {
        fprintf(stderr,"open src");
    }
    int fd_dst = open(dst,O_WRONLY|O_CREAT,mode);
    if(fd_dst < 0)
    {
        fprintf(stderr,"open dst");
    }
    char buf[1024] = {};
    int r;
    while((r = read(fd_src,buf,1024)) > 0)
    {
        write(fd_dst,buf,r);
        memset(buf,0x00,1024);
    }
    close(fd_src);
    close(fd_dst);
}//djis xa 
void cp_dir(const char* src, const char* dst)
{
    DIR* pdir = opendir(src);
    /if(pdir == NULL)
        fprintf(stderr,"open src dir");
    struct old_linux_dirent* pd = NULL;
}
int main(int argc,const char *argv[])
{
    if(argc != 3)
    {
        fprintf(stderr,"usage : %s src dst",argv[0]);
        exit(1);
    }
    //1.判断src是否存在
    struct stat buf; 
    if(stat(argv[1],&buf) == -1)
    {
        //不存在的情况
        fprintf(stderr,"src not exists!");
        exit(1);
    }
    else
    {
        //存在的情况 下来判断src的类型，是否为文件
        if((buf.st_mode & S_IFMT) == S_IFREG)
        {
        //是文件的情况
        //dst是否存在？
            struct stat dbuf;
            stat(argv[2],&dbuf);
            //dst不存在
            //创建dst文件。拷贝src文件。
            if(stat(argv[2],&dbuf) < 0)
            {
                cp_file(argv[1],argv[2],buf.st_mode);
                exit(0);
            }
            //dst存在，判断dst的类型？文件？目录？
            if((dbuf.st_mode & S_IFMT) == S_IFREG)
            {
                //dst是文件，判断是否要被覆盖
                printf("是否要覆盖%s文件  ",argv[2]);
                //dst是文件。判断是否要被覆盖
                //y覆盖，就拷贝src
                //n不覆盖就退出
                char cover = 'y';
                scanf("%c",&cover);
                if(cover == 'y' || cover == 'Y')
                {
                    truncate(argv[2],0);
                    cp_file(argv[1],argv[2],buf.st_mode);
                    exit(0);
                }
                else if(cover == 'n' || cover == 'N')
                {
                    exit(0);
                }
                else
                {
                    fprintf(stderr,"enter y or n!");
                    exit(1);
                }
            }
            else if((dbuf.st_mode & S_IFMT ) == S_IFDIR)
            {
                //dst是目录，在dst下创建一个新的文件，将src拷贝至该文件下。
                char tmp[] = {};
                sprintf(tmp,"%s/%s",argv[2],argv[1]);
                cp_file(argv[1],tmp,buf.st_mode);
            }
            else
            {
                //dst不是目录也不是文件的情况
                fprintf(stderr,"dst type error");
                exit(1);
            }
        }
        else
        {
            //不是文件的情况 判断是否为目录
            if((buf.st_mode&S_IFMT) == S_IFDIR)
            {
                //是目录  
                //判断dst是否存在
                struct stat dbuf;
                if(stat(argv[2],&dbuf) < 0)
                {
                    //不存在，创建一个新的目录，将src以子目录的方式拷贝到dst下
                    mkdir(argv[2],buf.st_mode);
                    cp_dir(argv[1],argv[2]);
                }
                else if(S_ISREG(dbuf.st_mode))
                {
                    fprintf(stderr,"dst type error");
                    exit(1);
                }
                else if(S_ISDIR(dbuf.st_mode))
                {
                    char tmp[1024] = {};
                    sprintf(tmp,"%s/%s",argv[2],argv[1]);
                    cp_dir(argv[1],tmp);
                }
            }
            else
            {
                //不是目录， 就退出
                exit(1);
            }
        }
    }
    return 0;
}
