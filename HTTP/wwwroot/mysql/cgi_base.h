#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
static int GetQueryString(char output[])
{
    //1. 先从环境变量中获取到方法
    char* method = getenv("REQUEST_METHOD");
    if(method == NULL)
    {
        fprintf(stderr,"REQUEST_METHOD failed\n");
        return -1;
    }
    //2. 如果是 GET 方法， 就是直接从环境变量中
    //    获取到 QUERY_STRING
    if(strcasecmp(method, "GET") == 0)
    {
        char* query_string = getenv("QUERY_STRING");
        if(query_string == NULL)
        {
            fprintf(stderr, "QUERY_STRING\n");
            return -1;
        }
        strcpy(output, query_string);
    }
    else
    {
        //3. 如果是 POST 方法， 先通过环境变量获取到
        //    CONTENT_LENGTH 再从标准输入中读取 body
        char* content_length_str = getenv("CONTENT_LENGTH");
        if(content_length_str == NULL)
        {
            fprintf(stderr, "CONTENT_LENGHT failes\n");
            return -1;
        }
        int content_length = atoi(content_length_str);
        int i = 0;
        // 表示当前已经往 output 中写了多少字符了
        for(;i < content_length; i++)
        {
            read(0, output + i, 1);
        }
        output[content_length] = '\0';
    }
    return 0;
}
