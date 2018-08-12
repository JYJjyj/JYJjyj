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
    //此时无论分隔符是什么，c 都成了\n
    //针对这两种情况，都把当前字符转换成\n
    //4. 如果当前字符是\n直接结束函数，（这一行已经读完）
        if(c == '\n')
        {
            break;
        }
    //5. 如果当前字符是一个普通字符，直接追加到输出结果中
        output[i++] = c;
    }
    output[i] = '\0';
    return i;   //返回i，当前缓冲区写了多少个字符
}

int Split(char input[], const char* split_char, char* output[])
{
    char* tmp = NULL;
    int output_index = 0;
    char* p = strtok(input, split_char);
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
        ssize_t read_size = ReadLine(new_sock, buf, )
    }
}

//完成具体的请求处理过程
void HandlerRequest(int64_t new_sock)
{
    HttpRequest req;
    memset(&req, 0, sizeof(req));
    //1.解析请求
    //  a) 从 socket中读取出首行
    //  char first_line[size] = {0};
    if(ReadLine(new_sock, req.first_line, sizeof(req.first_line) - 1) < 0)
    {
        
    }
    printf("first_line: %s\n",req.first_line);
    //  b) 解析首行，获取到 url 和 method
    if(ParseFirstLine(req.first_line, &req.method, &req.url) < 0)
    {

    }
    //  c) 解析url，获取到url_path 和 query_string
    if(ParseUrl(req.url, &req.url_path, &req.query_string) < 0)
    {

    }
    //  d) 解析 header，丢弃大部分 header，只保留Content-Length
    if(ParseHeader(new_sock, &req.content_length))
    {

    }
    //2.根据请求请求计算响应并写辉客户端
    if(strcasecmp(req.method, "GET") == 0 && req.query_string == NULL)
    {
        //处理静态页面
        HandlerStaticFile();
    }
    else if(strcasecmp(req.method, "GET") == 0 && req.query_string != NULL)
    {
        //处理动态页面
        HandlerCGI();
    }
    else if(strcasecmp(req.method, "POST") == 0)
    {
        //处理动态页面
        HandlerCGI();
    }
    else
    {
        //错误处理
    }
    //收尾工作,主动关闭socket, 会进入 TIME_WAIT
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
    HttpServerStart(argv[1], atoi(argv[2]))
    {

    }
}
