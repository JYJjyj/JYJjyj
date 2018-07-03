//带头结点的双向循环链表
//用模板进行泛型编程
#pragma once
#include <stdio.h>
#include <iostream>
#include <assert.h>
#include <string.h>
using namespace std;

template<class T>
struct ListNode
{
    T data;
    ListNode<T>* _prev;
    ListNode<T>* _next;

    ListNode()
        :data(T())
        ,_next(NULL)
        ,_prev(NULL)
    {}
};

template <class T>
class List
{
public:
   List()
    {
        _head = new ListNode<T>;
        _head->_next = _head;
        _head->_prev = _head;
    }
    List(const List<T>& l)
    {
        ListNode<T>* cur = l._head->_next;
        _head = CreateNode(T());
        _head->_next = _head;
        _head->_prev = _head;
        while(cur != l._head)
        {
            PushBack(cur->data);
            cur = cur->_next;
        }
    }
    List& operator=(List<T> l)
    {
        swap(_head,l._head);
    }
    void PushBack(const T& x)
    {
        Insert(_head,x);
    }
    void PopBack()
    {
        Erase(_head->_prev);
    }
    void PushFront(const T& x)
    {
        Insert(_head->_next,x);
    }
    void PopFront()
    {
        Erase(_head->_next);
    }
    void Insert(ListNode<T>* pos,const T& x)
    {
        ListNode<T> *newnode = CreateNode(x);
        newnode->_next = pos;
        newnode->_prev = pos->_prev;
        pos->_prev = newnode;
        newnode->_prev->_next = newnode;
    }
    void Erase(ListNode<T>* pos)
    {
        assert(pos != _head);
        ListNode<T>* next = pos->_next;
        ListNode<T>* prev = pos->_prev;
        delete pos;
        next->_prev = prev;
        prev->_next = next;
    }
    bool Empty()
    {
        if(_head->_next == _head)
            return true;
        else
        {
            return false;
        }
    }
    const T& Top()
    {
        return _head->_next->data;
    }
    void show()
    {
        ListNode<T>* cur = _head->_next;
        while(cur != _head)
        {
            cout<<cur->data<<" ";
            cur = cur->_next;
        }
        cout<<endl;
    }
    ~List()
    {
        ListNode<T> *cur = _head->_next;
        while(cur != _head)
        {
            ListNode<T>* next = cur->_next;
            delete cur;
            cur = next;
        }
    }
    
protected:
    ListNode<T>* CreateNode(const T& x)
    {
        ListNode<T>* newnode = new ListNode<T>;
        newnode->data = x;
        newnode->_next = NULL;
        newnode->_prev = NULL;
    }
    ListNode<T>* _head;
};
