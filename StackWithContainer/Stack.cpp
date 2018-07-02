#include <iostream>
#include <stdio.h>
#include "../Vector/Vector.h"
using namespace std;

template <class T ,template<class>class Container = Vector>
class Stack
{
public:
    void Push(const T& x)
    {
        _con.PushBack(x);
    }
    void Pop()
    {
        _con.PopBack();
    }
    const T& Top()
    {
        _con.Top();
    }
    bool Empty()
    {
       _con.Empty(); 
    }
    void show()
    {
        _con.show();
    }
protected:
    Container<T> _con;
};

int main()
{
    Stack<int,Vector> s;
    s.Push(2);
    s.Push(3);
    s.Push(4);
    s.Push(5);
    s.show();
    cout<<"栈顶元素："<<s.Top()<<endl;
    s.Pop();
    s.show();
    Stack<int,Vector> s1;
    s1 = s;
    s1.show();
    return 0;
}
