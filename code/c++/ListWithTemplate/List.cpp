#include "List.h"

void test()
{
    List<string> l;
    l.PushBack("jofifdjs");
    l.PushBack("d98dehwu");
    l.PushBack("23435");
    l.PushBack("0odjcs98");
    l.show();
    l.PopBack();
    l.show();
    l.PushFront("123456");
    l.show();
    l.PopFront();
    l.show();
    List<string> l1(l);
    l1.show();
    l1.PushBack("jiduhv");
    l1.show();
    l1 = l;
    l1.show();
}

int main()
{
    test();
    return 0;
}
