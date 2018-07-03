#include <iostream>
#include<assert.h>
#include<stdio.h>
#include<string.h>
typedef int DataType;

class Vector
{
public:
	Vector()
	{
		_first = new DataType[3];
		_finish = _first;
		_endofstorage = _first + 3;
	}
	Vector(const Vector& v)
	{
		_first = new DataType[v.Size()];
		memmove(_first, v._first, v.Size()*sizeof(DataType));
		_finish = _first + v.Size() ;
		_endofstorage = _finish ;
	}
	Vector& operator=(Vector v);
	~Vector()
	{
		delete[] _first;
	}
	size_t Size() const
	{
		return _finish - _first ;
	}
	size_t Capacity() const
	{
		return _endofstorage - _first ;
	}
	void Expand(size_t n);
	void PushBack(DataType x);
	void Reserve(size_t n);
	void PopBack();
	void Insert(size_t pos, DataType x);
	void Erase(size_t pos);
    size_t Find(DataType x);
    void Print();
private:
	DataType* _first;
	DataType* _finish;
	DataType* _endofstorage;
};
