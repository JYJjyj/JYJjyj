/////////////////////////////////////////////////////////////////////
//该文件包含了整个 HTTP 服务器的实现
/////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/sendfile.h>
#include <sys/wait.h>
typedef struct sockaddr_in sockaddr_in;
typedef struct sockaddr sockaddr;

#define SIZE (1024 * 4)
typedef struct HttpRequest
{
    char first_line[SIZE];
    char* method;
    char* url;
    char* url_path;
    char* query_string;
    int content_length;
}HttpRequest;

//从sock中读取一行数据
////HTTP请求中换行符是什么？
//(常见的三种)  \n  \r  \r\n
//核心思路，把未知问题转换成已知问题，把\r \r\n 往\n上转换
int ReadLine(int sock, char output[], ssize_t max_size)
{
    //1. 一个字符一个字符的从socket中读取数据
    char c = '\0';
    ssize_t i = 0;
    while(i < max_size)
    {
        ssize_t read_size = recv(sock, &c, 1, 0);
        if(read_size <= 0)
        {
            //由于此处是希望能读到一个完整的行，如果还没读到换行
            //就读到了EOF，此时就认为出错了
            return -1;
        }
    //2. 判定当前字符是不是\r
    //3. 如果当前字符是\r , 就尝试读取下一个字符
        if(c == '\r')
        {
            recv(sock, &c, 1, MSG_PEEK);  // MSG_PEEK提前看缓冲区的内容，但是不将该
        //    a) 如果下一个字符是 \n
        //    将\r\n转换为\n
            if(c == '\n')
            {
                recv(sock, &c, 1, 0);
            }
        //    b) 如果下一个字符不是 \n
            else 
            {
                c = '\n';
            }
        }
        output[i++] = c;
    //此时无论分隔符是什么，c 都成了\n
    //针对这两种情况，都把当前字符转换成\n
    //4. 如果当前字符是\n直接结束函数，（这一行已经读完）
        if(c == '\n')
        {
            break;
        }
    //5. 如果当前字符是一个普通字符，直接追加到输出结果中
    }
    output[i] = '\0';
    return i;   //返回i，当前缓冲区写了多少个字符
}

int Split(char input[], const char* split_char, char* output[])
{
    char* tmp = NULL;
    int output_index = 0;
    char* p = strtok_r(input, split_char,&tmp);
    while(p != NULL)
    {
        output[output_index++] = p;
        //strtok 会在全局区有一块空间，用于保存上次切割到哪了，所以可能会有线程安全问题
        //strtok_r是线程安全版本
        p = strtok_r(NULL, split_char, &tmp);
    }
    return output_index;
}

//解析首行， 获取到其中的method 和 url
//首行格式：
//暂时先不考虑带域名的情况的url（偷懒）
//GET /index.html?a=10&b=20 HTTP/1.1
int ParseFirstLine(char first_line[],char** p_method, char** p_url)
{
    char *tok[10] = {0};
    //使用Split函数对字符串进行切分， n表示切分结果有几部分
    int n = Split(first_line, " ", tok);
    if(n != 3)
    {
        printf("Split failed! n = %d\n", n);
        return -1;
    }
    //此处还可以进行更复杂的校验
    *p_method = tok[0];
    *p_url = tok[1];
    return 0;
}

//url 形如 ：/index.html?a=10&b=20
//http://www.baidu.com/index.html?a=10&b=20(暂时不考虑)
int ParseUrl(char url[], char** p_url_path, char** p_query_string)
{
    *p_url_path = url;
    // 查找 ? 所在的位置
    char* p = url;
    for(; *p != '\0'; ++p)
    {
        if(*p == '?')
        {
            //找到了 ？，此时就可以把 ？替换成'\0'
            *p = '\0';
            *p_query_string = p + 1;
            return 0;
        }
    }
    //http://www.baidu.com也没有？，所以不能直接返回错误.
    *p_query_string = NULL;
    return 0;
}

int ParseHeader(int new_sock, int* content_length)
{
    char buf[SIZE] = {0};
    while(1)
    {
        ssize_t read_size = ReadLine(new_sock, buf, sizeof(buf) - 1);
        if(read_size <= 0)
        {
            return -1;
        }
        if(strcmp(buf, "\n") == 0)
        {
            return 0;
        }
        //content-length : 100\n
        const char* key = "Content-Length: ";
        if(strncasecmp(buf, key, strlen(key)) == 0)
        {
            *content_length = atoi(buf + strlen(key));
            //此处的不可以使用break，否则就是粘包问题
            //break;
        }
    }
    return 0;
}

void Handler404(int new_sock)
{
    const char* first_line = "HTTP/1.1 404 Not Found\n";
    //此处的代码可以是先不加Header部分
    //content-type 可以让浏览器自动识别
    //content-length 可以通过关闭socket 
    //的方式告知浏览器已经读完了
    //
    //body部分就是 html 页面
    
    const char* blank_line = "\n";
    const char* body = "<head><meta http-equiv=\"Content-Type\" content=\"text/html;charset=utf-8\"></head>\
                        <h1>404!!!~~~~~~~~a hahahahaha   你是一只大傻猪~~~~~~~~~</h1>";
    send(new_sock, first_line, strlen(first_line), 0);
    send(new_sock, blank_line, strlen(blank_line), 0);
    send(new_sock, body, strlen(body), 0);
    return; 
}

int isDir(const char* file_path)
{
    struct stat st;
    int ret = stat(file_path, &st);
    if(ret < 0)
    {
        // 此处不是目录
        return 0;
    }
    if(S_ISDIR(st.st_mode))
    {
        return 1;
    }
    return 0;
}

ssize_t GetFileSize(const char* file_path)
{
    struct stat st;
    int ret = stat(file_path, &st);
    if(ret < 0)
        return 0;
    return st.st_size;
}

int WriteStaticFile(int new_sock, const char* file_path)
{
    //1. 打开文件。如果打开失败，就返回404
    int fd = open(file_path, O_RDONLY);
    if(fd < 0)
    {
        perror("open");
        return 404;
    }
    //2. 构造 HTTP 响应报文
    const char* first_line = "HTTP/1.1 200 OK\n";
    send(new_sock, first_line, strlen(first_line), 0);
    // 此处如果从一个严谨的角度考虑需要加上一些 header
    // 此处没有写content-type 是因为浏览器能够自动识别
    // 没有写content-length 是因为后面立刻关闭了 socket
    // 浏览器就能识别出什么时候结束
    const char* blank_line = "\n";
    send(new_sock, blank_line, strlen(blank_line), 0);
    //3. 读文件内容并且写到 socket 之中
    //这里我们使用的是更高效的 sendfile 来完成文件传输操作
    //char c = '\0';
    //while(read(new_sock, &c, 1) > 0)
    //{
    //    send(new_sock, &c, 1, 0);
    //}
    ssize_t file_size = GetFileSize(file_path);
    sendfile(new_sock, fd, NULL, file_size);
    //4. 关闭文件
    close(fd);
    return 200;
}

void HandlerFilePath(const char* url_path, char file_path[])
{
    //url_path 是以 / 开头的，所以不需要 wwwroot 之后显式指明 /
    sprintf(file_path, "./wwwroot%s", url_path);
    //strcat(file_path, url_path);
    // 如果 url_path 指向的目录，就在目录后面拼装上 index.html 作为默认访问的文件
    // 如何识别url_path 指向的文件是普通文件还是目录呢？
    // a) url_path 以 / 结尾，例如：/image/
    if(file_path[strlen(file_path) - 1] == '/')
    {
        strcat(file_path, "index.html");
    }
    else
    {
        // b) url_path 没有以 / 结尾，此时需要根据文件属性来判定是否是目录
        if(isDir(file_path))
        {
            strcat(file_path, "/index.html");
        }
    }
}

int HandlerStaticFile(int new_sock, const HttpRequest* req)
{
    //1.根据 url_path 获取到文件的真实路径
    //例如，此时 HTTP 服务器的根目录叫做 ./wwwroot
    //此时有一个文件 ./wwwroot/image/101.jpg
    //在 url 中写 path 就叫做 /image/101.jpg
    char file_path[SIZE] = {0};
    //根据下面的函数把 /image/yyqx.jpg 转换成了
    //磁盘上的 ./wwwroot/yyqx.jpg
    HandlerFilePath(req->url_path, file_path);
    //2.打开文件，读取文件内容，把文件内容写到 socket 中
    int err_code = WriteStaticFile(new_sock, file_path);
    return err_code;
}

int HandlerCGIFather(int new_sock, int father_read, int father_write, const HttpRequest* req)
{
    //  a) 如果是 POST 请求，把 body 部分的数据都出来写到管道中
    //      剩下的动态生成页面的过程都交给子进程来完成
    if(strcasecmp(req->method, "POST") == 0)
    {
        char c ='\0';
        int i = 0;
        // 此处不可以使用sendfile函数，因为sendfile要求目标文件描述符必须是一个socket
        //为什么不一次将所有的数据全部读出来？
        //若是 body 很长，那么read是很可能在读写过程中被信号打断
        //就会导致 body 没有被完全读完
        //即使缓冲区足够长， read还是很可能被打断
        for(; i < req->content_length; i++)
        {
            read(new_sock, &c, 1);
            write(father_write, &c, 1);
        }
    }
    //  b) 构造 HTTP 响应中的首行， header，空行
    const char* first_line = "HTTP/1.1 200 OK\n";
    send(new_sock, first_line, strlen(first_line), 0);
    //此处先不管header， 浏览器可以自动识别header，也会知道什么时候结束
    //为了简单先不管。
    const char*blank_line = "\n"; 
    send(new_sock, blank_line, strlen(blank_line), 0);
    
    //  c) 从管道中读取数据（自数据动态生成的页面），把这个数据也写到socket之中
    //     此处也不能使用 sendfile， 因为数据的长度不容易确定。所以还是使用循环来
    char c = '\0';
    while(read(father_read, &c, 1) > 0)
    {
        write(new_sock, &c, 1);
    }
    //  d) 进程等待，回收子进程的资源。
    //  此处如果要进行进程等待，那么最好使用waitpid
    //  保证当前线程回收的子进程就是自己当时创建的那个子进程
    //  更简洁的做法，是直接使用忽略 SIGCILD 信号(在 main 函数中)
    return 200;
}

int HandlerCGIChild(int child_read, int child_write, const HttpRequest* req)
{
    //  a) 设置环境变量 (METHOD, CONTENT_LENGTH, QUERY_STRING)
    //      如果把上面这几个信息通过管道来告知替换之后的程序。
    //      也是完全可行的，但是此处我们是要遵守 CGI 标准。所以
    //      必须使用环境变量传递以上的信息
    //      注意！！！ 设置环境变量的步骤，不能由父进程来进行
    //      虽然子进程能够继承父进程的环境变量，由于同一时刻
    //      会有多个请求，每个请求都在尝试修改父进程的环境变量
    //      就会产生类似于线程安全的问题，导致子进程不能正确的
    //      获取到这些信息
    char method_env[SIZE] = {0};
    sprintf(method_env, "REQUEST_METHOD=%s", req->method);
    putenv(method_env);
    if(strcasecmp(req->method, "GET") == 0)
    {
        //设置 QUERY_STRING
        char query_string_env[SIZE] = {0};
        sprintf(query_string_env, "QUERY_STRING=%s", req->query_string);
        putenv(query_string_env);
    }
    else
    {
        //设置 CONTENT_LENGTH
        char content_length_env[SIZE] = {0};
        sprintf(content_length_env, "CONTENT_LENGTH=%d", req->content_length);
        putenv(content_length_env);
    }
    //  b) 把标准输入和标准输出重定向到管道上.此时CGI程序读写标准
    //      输入输出就相当于读写管道
    //      由于CGI机制是不限定编程语言的，所以，有些语言在访问管道时
    //      就会有困难，为了方便读写就需要重定向
    dup2(child_read, 0);
    dup2(child_write, 1);
    //  c) 子进程进行程序替换（需要先找到是哪个CGI可执行程序，然后
    //      再使用 exec 函数进行替换）
    //      替换成功之后，动态页面完全交给 CGI 程序进行计算生成
    //      假设 url_path 值为 /cgi-bin/test
    //      说明对应的 CGI 程序的路径就是 ./wwwroot/cgi-bin/test
    char file_path[SIZE] = {0};
    HandlerFilePath(req->url_path, file_path);
    // 第一个参数可执行程序的路径
    // 第二个参数，argv[0]
    // 第三个参数传NULL，表示命令行参数结束了
    execl(file_path, file_path, NULL);
    //  d) 替换失败的错误处理, 子进程就是为了替换而生的
    //      如果替换失败，子进程也就没有存在的必要了，
    //      反而如果子进程继续存在，继续执行父进程原有的代码，
    //      就有可能对父进程原有的逻辑造成干扰
    exit(0);
}

int HandlerCGI(int new_sock, const HttpRequest* req)
{
    int err_code = 200;
    // 1. 创建一对匿名管道
    int fd1[2], fd2[2];
    pipe(fd1);
    pipe(fd2);
    int father_read = fd1[0];
    int child_write = fd1[1];
    int father_write = fd2[1];
    int child_read = fd2[0];
    // 2. 创建子进程 fork 
    pid_t ret = fork();
    if(ret > 0)
    {
    // 3. 父进程核心流程：
        //father
        //此处先把不必要的文件描述符关闭掉
        //为了保证后面父进程从管道中读数据的时候，read能够正确
        //返回不阻塞。后面的代码中会循环从管道中读数据，读到EOF
        //就认为读完了，循环退出，而对于管道来说，必须所有的写端
        //关闭，再进行读写，才是读到EOF
        //而这里所有的写端包含父进程的写端和子进程的写端
        //子进程的写端会随着子进程的终止而自动关闭
        //父进程的写端，就可以在此处，直接关闭
        //（父进程不需要使用这个写端）
        close(child_write);
        close(child_read);
        HandlerCGIFather(new_sock, father_read, father_write, req);
    }
    else if(ret == 0)
    {
        //child
        // 4.子进程核心流程：
        close(father_write);
        close(father_read);
        HandlerCGIChild(child_read, child_write, req);
    }
    else
    {
        perror("fork");
        err_code = 404;
        goto END;
        //error
    }
    //  
END:
    //收尾工作
    close(father_read);
    close(father_write);
    close(child_read);
    close(child_write);
    return err_code;
}

//完成具体的请求处理过程
void HandlerRequest(int64_t new_sock)
{
    int err_code = 200;
    HttpRequest req;
    memset(&req, 0, sizeof(req));
    //1.解析请求
    //  a) 从 socket中读取出首行
    //  char first_line[size] = {0};
    if(ReadLine(new_sock, req.first_line, sizeof(req.first_line) - 1) < 0)
    {
        //错误处理，此处一旦触发错误处理逻辑
        //此处我们就只返回404的数据报
        //正常来说，根据不同的错误原因返回不同的数据报
        err_code = 404;
        goto END; 
    }
    printf("first_line: %s\n",req.first_line);
    //  b) 解析首行，获取到 url 和 method
    if(ParseFirstLine(req.first_line, &req.method, &req.url) < 0)
    {
        err_code = 404;
        goto END; 
    }
    //  c) 解析url，获取到url_path 和 query_string
    if(ParseUrl(req.url, &req.url_path, &req.query_string) < 0)
    {
        err_code = 404;
        goto END; 
    }
    //  d) 解析 header，丢弃大部分 header，只保留Content-Length
    if(ParseHeader(new_sock, &req.content_length))
    {
        err_code = 404;
        goto END; 
    }
    //2.根据请求计算响应并写回客户端
    if(strcasecmp(req.method, "GET") == 0 && req.query_string == NULL)
    {
        //处理静态页面
        err_code = HandlerStaticFile(new_sock, &req);
    }
    else if(strcasecmp(req.method, "GET") == 0 && req.query_string != NULL)
    {
        //处理动态页面
        err_code = HandlerCGI(new_sock, &req);
    }
    else if(strcasecmp(req.method, "POST") == 0)
    {
        //处理动态页面
        err_code = HandlerCGI(new_sock, &req);
    }
    else
    {
        //错误处理
        err_code = 404;
        goto END; 
    }
    //收尾工作,主动关闭socket, 会进入 TIME_WAIT
END:
    if(err_code != 200)
        Handler404(new_sock);
    close(new_sock);
}

void* ThreadEntry(void *arg)
{
    int64_t new_sock = (int64_t)arg;
    HandlerRequest(new_sock);
    return NULL;
}

void HttpServerStart(const char* ip, short port)
{
    //1.基本的初始化，基于 TCP
    int listen_sock = socket(AF_INET, SOCK_STREAM, 0);
    if(listen_sock < 0)
    {
        perror("socket");
        return;
    }

    //设置一个选项
    int opt = 1;
    setsockopt(listen_sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(ip);
    addr.sin_port = htons(port);
    int ret = bind(listen_sock, (sockaddr*)&addr, sizeof(addr));
    if(ret < 0)
    {
        perror("bind");
        return;
    }

    ret = listen(listen_sock, 5);
    if(ret < 0)
    {
        perror("listen");
        return;
    }
    printf("HttpServer start OK!\n");
    //2.进入事件循环
    while(1)
    {
        //实现一个多线程版本的服务器，
        //每个请求都创建一个新的线程
        sockaddr_in peer;
        socklen_t len = sizeof(peer);
        int64_t new_sock = accept(listen_sock, (sockaddr*)&peer, &len);
        if(new_sock < 0)
        {
            perror("accept");
            continue;
        }
        pthread_t tid;
        //由于是多线程服务器，所以new_sock传地址会让多个线程共用一个new_sock
        //所以我们需要传值，让每个线程都有自己的new_sock
        pthread_create(&tid, NULL, ThreadEntry, (void*)new_sock);
        pthread_detach(tid);
    }
}

int main(int argc, char* argv[])
{
    if(argc != 3)
    {
        printf("usage ./http_server [ip] [port]\n");
        return 1;
    }
    signal(SIGCHLD, SIG_IGN);
    HttpServerStart(argv[1], atoi(argv[2]));
    return 0;
}
