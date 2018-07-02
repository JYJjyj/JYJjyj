// 1.现代写法 
// 2.传统写法 
// 3.字符串增删查改 
#include <iostream>
#include <stddef.h>
#include <string.h>
#include<assert.h>
#include<memory.h>
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
    if(_size>=_capacity)
        Expand(2*_capacity);
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
    if(_size>=_capacity)
        Expand(_size + len + 1);
    if(_size + len > _capacity)
        Expand(_size + len +1);
    for(int i = _size + 1; i >= (int)pos; i--)
    {
        _str[i + len] = _str[i];
    }
    strncpy(_str + pos,str,len);
    _size += len; 
}
//在任意位置处删除n个元素
void Erase(size_t pos, size_t n = 1)
{
    if(pos > _size)
        return;
    strcpy(_str + pos, _str + pos + n);
    _size -= n;
}
//将字符串中的所有字符ch1全部替换成字符ch2
void Replace(char ch1, char ch2)
{
    size_t size = _size;
    char *str = _str;
    while(size--)
    {
        if(*str == ch1)
            *str = ch2;
        else
            str++;
    }
}


//替换_str中的第一个sub1为sub2
void Replace(const char* sub1, const char* sub2)
{
    if(sub1 == NULL || sub2 == NULL)
        return;
    size_t len = strlen(sub1);
    size_t len1 = strlen(sub2);
        if(char *tmp = strstr(_str,sub1))
        {
            memmove(tmp+len1,tmp+len,(_str+_size+1)-tmp+len);
            strncpy(tmp,sub2,len1);
            return;
        }
}
   
void Replace1(const char* sub1, const char* sub2)
{
    if(sub1 == NULL || sub2 == NULL)
        return;
    char *ret = _str;
    size_t size = 0;
    size_t len = strlen(sub1);
    size_t len1 = strlen(sub2);
    while(size<=_size)
    {   if(char *tmp = strstr(ret,sub1))
        {
            memmove(tmp+len1,tmp+len,(ret+_size+1)-ret+len);
            strncpy(tmp,sub2,len1);
            ret += len1;
            size += len1;
            return;
        }    
        else
        { 
            size++;
            ret++;
        }           
    }
}
size_t Find(char ch)
{
    size_t size = 0;
    while(size <= _size)
    {
       if( _str[size] == ch )
           return size;
       else
           size++;
    }
    return -1;
}
//查找是否存在字符串str
size_t Find(const char* str)
{
    if(NULL == str)
        return -1;
    size_t len = strlen(str);
    for(int i = 0; i < (int)_size; i++)
    {
        if(strncmp(_str+i,str,len)==0)
            return i;
    }
    return -1;
}
// s1 + 'a' 
 String operator+(char ch)
{
    String s(*this);
    s.Insert(s._size,ch);
    return s;
}
 String& operator+=(char ch)
{
    Insert(_size,ch);
    return *this;
}
 String operator+(const char* str)
{
    String s(*this);
    s.Insert(s._size,str);
    return s;
}
 String& operator+=(const char* str)
{
    Insert(_size,str);
    return *this;
}

// bool operator>(const String& s)
//{
//    if(strcmp(_str,s._str) > 0)
//        return true;
//    else
//        return false;
//}
 bool operator>(const String& s)
{
    char *str = _str;
    char *str1 = s._str;
    while(*str != '\0' && *str1 != '\0')
    {
        if(*str == *str1)
        {
            str++;
            str1++;
        }
        else if(*str > *str1 )
            return true;
        else
            return false; 
    }
    if(*str != '\0')
        return true;
    else
        return false;
}
 bool operator>=(const String& s)
{
    return (*this > s||*this == s);
}
 bool operator<(const String& s)
{
    return !(*this >= s);
}
 bool operator<=(const String& s)
{
    return !(*this > s);
}
// bool operator==(const String& s)
//{
//    if(strcmp(_str,s._str) == 0)
//        return true;
//    else
//        return false;
//}
bool operator==(const String& s)
{
    char *str = _str;
    char *str1 = s._str;
    while(*str != '\0'&&*str1 != '\0')
    {
        if(*str == *str1)
        {
            str++;
            str1++;
        }
        else
            return false;
    }
    if(*str == '\0' && *str1 == '\0')
        return true;
    else
        return false;
}
 bool operator!=(const String& s)
{
    return !(*this == s);
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
    s2.Erase(0);
    cout<<s2;
    s2.Replace('l','o');
    cout<<s2;
    size_t ret;
    ret = s2.Find('o');
    cout<<"下标为："<<ret<<endl;
    ret = s2.Find("123");
    cout<<"下标为："<<ret<<endl;
     String s4(s1);
     s4 = s1 + '!';
    cout<<s4;
   s4 += 'a';
       cout<<s4;
           s4 += "aaaa";
               cout<<s4;
  s4 = s1 + "!!!";
      cout<<s4;
      String s5(s1);
      cout<<"s1 : "<<s1;
      cout<<"s2 : "<<s2;
      cout<<"s3 : "<<s3;
      cout<<"s4 : "<<s4;
      cout<<"s5 : "<<s5;
      cout<<(s5>s1)<<endl;
      cout<<(s5>=s1)<<endl;
      cout<<(s5<s1)<<endl;
      cout<<(s5<=s1)<<endl;
      cout<<(s5==s1)<<endl;
      cout<<(s5!=s1)<<endl;
      s2.Replace("oo","1");
      cout<<s2;
      s1.Replace1("el","222");
      cout<<s1;
    return 0;
}
