#include "Vector.h"

void testInsert()
{
    Vector<char> v;
    v.Insert(0,'a');
    v.Insert(1,'b');
    v.show();
}
void testErase()
{
    Vector<string> v;
    v.Insert(0,"fihwuehds");
    v.Insert(1,"dijwiufhdc");
    v.Insert(0,"kdwojcs");
    v.show();
    v.PopBack();
    v.show();
}
void testFind()
{
    Vector<string> v;
    v.PushBack("This ");
    v.PushBack("is ");
    v.PushBack("a ");
    v.PushBack("statement!");
    cout<<v.Find("a")<<endl;
}
int main()
{
   testInsert();
   testErase();
   testFind();
   return 0;
}
