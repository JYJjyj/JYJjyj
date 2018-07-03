#include <stdio.h>
#include <iostream>
#include <string.h>
#include <string>
#include <assert.h>
using namespace std;
template <class T>
class Vector
{
public:
    Vector()
    {
        _start = new T[10];
        _finish = _start;
        _endofstorage = _start + 10;
    }
    Vector(const Vector<T>& v)
    {
        _start = new T[v.Size()];
        size_t i = 0;
        for(; i < v.Size; i++)
        {
            _start[i] = v._start[i];
        }
        _finish = _start + v.Size();
        _endofstorage = _finish;
    }
    Vector<T>& operator=(Vector<T> v)
    {
        _start = new T[v.Size()];
        swap(_start,v._start);
        swap(_finish,v._finish);
        swap(_endofstorage,v._endofstorage);
        return *this;
    }
    void PushBack(const T& x)
    {
        Insert(0,x);
    }
    void PopBack()
    {
        Erase(Size() - 1);
    }
    void Expand(size_t n)
    {
        T* old = _start;
        size_t size = Size();
        _start = new T[n];
        _finish = _start + size;
        _endofstorage = _start + n;
        delete[] old;
    }
    void Insert(size_t pos,const T& x)
    {
        if(Size() >= Capacity())
            Expand(2 * Capacity());
        size_t i = pos;
        for(;i < Size(); i++)
        {
            _start[i+1] = _start[i];
        }
        _start[pos] = x;
        _finish++;
    }
    void Erase(size_t pos)
    {
        assert(pos < Size());
        size_t i = pos;
        for(;i < Size()-1; i++)
        {
            _start[i] = _start[i+1];
        }
        _finish--;
    }
    void reserve(size_t n)
    {
        if(n > Capacity())
            Expand(n);
        else if(n < Capacity())
            _finish = _start + (n > Size() ? n : Size());
        else
        {
            return ;
        }
    }
    size_t Size() const
    {
        return _finish - _start;
    }
    size_t Capacity() const
    {
        return _endofstorage - _start;
    }
    size_t Find(const T& x)
    {
        size_t i = 0;
        for(; i<Size();i++)
        {
            if(_start[i] == x)
                return i;
        }
        return -1;
    }
    ~Vector()
    {
        delete[] _start;
        _start = NULL;
        _finish = _endofstorage = _start;
    }
    void show()
    {
        size_t i = 0;
        for(;i <Size(); i++)
        {
            cout<<_start[i]<<" ";
        }
        cout<<endl;
    }
protected:
    T* _start;
    T* _finish;
    T* _endofstorage;
};
