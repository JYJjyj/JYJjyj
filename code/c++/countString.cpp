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
     :_str(new char[strlen(str)+5])
 {
     *(int*)(_str)=1;
     _str = _str + 4;
     strcpy(_str,str);
 }
 // s2(s1) 
 String(const String& s) 
 {
     _str = s._str;
    (*(int*)(_str-4))++;
 }
 //s2 = s1 
 String& operator=(const String& s)
 {
     if(s._str != _str)
     { 
        if(*(int *)(_str - 4) == 1)
            delete[] (_str - 4);
        _str = s._str;
        *(int *)(_str - 4) += 1;
     }  
        return *this;
 }
 ~String() 
 {
     if(*(int *)(_str - 4) == 0)
         delete[] (_str-4);
 }
 const char* c_str()
 {
     char* ret = new char[strlen(_str)+1];
     strcpy(ret,_str);
     return ret;
 }
 void CopyOnWrite()
 {
     char *tmp = new char[strlen(_str)+5];
     *((int*)tmp) = 1;
     *((int *)(_str)-1) -= 1;
     tmp += 4;
     strcpy(tmp,_str);
     _str = tmp;
 }
 char& operator[](size_t pos)
 {
     return _str[pos];
 }
friend ostream& operator<<(ostream& out,const String& s);
 private: 
 char* _str; // 引用计数在头上 
 }; 
ostream& operator<<(ostream& out,const String& s)
{
    out<<"_str: "<<s._str<<endl;
    out<<"count: "<<*((int*)(s._str-4))<<endl;
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
    printf("%s\n",s1.c_str());
    return 0;
}
