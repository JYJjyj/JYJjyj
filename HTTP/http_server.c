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
    *p_url_path = NULL;
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

int HandlerCGI()
{
    return 404;
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
        err_code = HandlerCGI();
    }
    else if(strcasecmp(req.method, "POST") == 0)
    {
        //处理动态页面
        err_code = HandlerCGI();
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
    HttpServerStart(argv[1], atoi(argv[2]));
    return 0;
}
