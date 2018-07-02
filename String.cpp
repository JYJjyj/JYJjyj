// 1.现代写法 
// 2.传统写法 
// 3.字符串增删查改 
#include <iostream>
#include <stddef.h>
#include <string.h>
#include<assert.h>
using namespace std;
#define MaxSize 16
class String 
{ 
public:
String(const char* str ="")
    :_str(new char[strlen(str) + 1]),
    _size(strlen(str)),
    _capacity(MaxSize)
{
    strcpy(_str,str);
}
void Swap(String& s)
{
    String ret(s);
    _str = ret._str;
    _size = ret._size;
    _capacity = ret._capacity;
}
friend ostream& operator<<(ostream& out,String &str);
String(const String& s)
    :_str(new char[s._size + 1])
    ,_size(s._size)
    ,_capacity(s._capacity)
{
    strcpy(_str,s._str);
}
String& operator=(String s)
{
    _str = new char[s._size + 1];
    _capacity = s._size + 1;
    _size = s._size;
    strcpy(_str,s._str);
    return *this;
}
//现代写法
//String& operator=(String s)
//{
//    String ret(s);
//    _str = ret._str;
//    _size = ret._size;
//    _capacity = ret._capacity;
//    return *this;
//}
~String()
{
    delete[] _str;
}
const char* c_str()
{
    char* ret = new char[_size + 1];
    ret = strcpy(ret,_str);
    *(ret+_size) = '\0';
    return (const char*)ret;
}
void Expand(size_t n)
{
    char *tmp = _str;
    _str = new char[n];
    strcpy(_str,tmp);
    _capacity = n;
}
void PushBack(char ch)
{
    if(_size>=_capacity)
        Expand(2*_capacity);
    *(_str+_size)=ch;
    *(_str + _size + 1)='\0';
    _size++;
}
void PushBack(const char* str)
{
    assert(str);
   size_t len = strlen(str);
   if(_size>=_capacity)
        Expand(_capacity + len);
    strcpy(_str+_size,str);
    _size += len;
    *(_str + _size + 1) = '\0';
}
//尾删一个元素
void PopBack()
{
    *(_str + --_size)='\0';
}
//在给定位置插入一个元素
void Insert(size_t pos, char ch)
{
    if(pos > _size + 1)
        return;
    for(int i = _size + 1; i >= (int)pos; i--)
    {
       _str[i+1] = _str[i];
    }
    _str[pos] = ch;
    ++_size;
}
void Insert(size_t pos, const char* str)
{
    if(pos > _size + 1)
        return;
    size_t len;
    len = strlen(str);
    if(_size + len > _capacity)
        Expand(_size + len +1);
    for(int i = _size + 1; i >= (int)pos; i--)
    {
        _str[i + len] = _str[i];
    }
    strncpy(_str + pos,str,len);
    _size += len; 
}
private: 
char* _str; 
size_t _size; 
size_t _capacity; 
}; 

ostream& operator<<(ostream& out,String &str)
{
    out<<str._str<<endl;
    return out;
}
int main()
{
    String s1("hello world!!!!! ");
    cout<<s1;
    String s2(s1);
    cout<<s2;
    String s3("s3:world");
    cout<<s3;
    s3 = s2;
    cout<<"s3::"<<s3;
    s1.PushBack('!');
    cout<<s1;
    cout<<s2;
    s2.PushBack("abc");
    cout<<s2;
    s2.PushBack('x');
    cout<<s2;
    s2.PopBack();
    cout<<s2;
    s2.Insert(3,'l');
    cout<<s2;
    s2.Insert(7,"hello ");
    cout<<s2;
    return 0;
}
