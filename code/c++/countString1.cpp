#include <iostream>
#include<assert.h>
#include<stdio.h>
#include<assert.h>
#include<string.h>
using namespace std;
class String 
{ 
public: 
String(const char* str = "")
   :_str(new char[strlen(str)+1])
    ,_pCount(new size_t(1))
    {
        strcpy(_str,str);
    }
// s2(s1) 
 String(const String& s)
     :_str(s._str)
     ,_pCount(s._pCount)
{
    (*_pCount)++;
}
 //s2 = s1 
 String& operator=(const String& s)
{
    if(s._str != _str)
    {
        if(*_pCount == 1)
        {
            delete[] _str;
        }
        else
        {
            (*_pCount)--;
        }
            _str = s._str;
            _pCount = s._pCount;
            (*_pCount)++;
    }
        return *this;
}
 ~String()
{
    if(*_pCount == 1)
    {
        *(_pCount) = 0;
        delete[] _str; 
        delete _pCount;
    }
}
 const char* c_str()
{
    char *ret ;
    ret = new char[strlen(_str)]+1;
    ret = strcpy(ret,_str);
    return ret;
}
//写时拷贝
 void CopyOnWrite()
{
    char *ret = new char[strlen(_str)+1];
    strcpy(ret,_str);
    _str = ret;
    (*_pCount)--;
    *_pCount = 1;
}
 char& operator[](size_t pos)
{
    assert(pos>strlen(_str));
    return _str[pos];
}
 friend ostream& operator<<(ostream& out,const String& s);
 private: 
 char* _str; 
 size_t* _pCount; 
 }; 
ostream& operator<<(ostream& out,const String& s)
{
    out<<"_str: "<<s._str<<endl;
    out<<"_pCount: "<<*(s._pCount)<<endl;
    return out;
}
int main()
{
    String s1("abcd");
    String s2(s1);
    cout<<s1;
    cout<<s2;
    String s3("feg");
    s3 = s1;
    cout<<s3;
    cout<<s2;
    s1.CopyOnWrite();
    cout<<s1;
    //printf("%s\n",s1.c_str());
    return 0;
}
