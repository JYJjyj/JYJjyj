#include"Vector_List1.h"

	Vector& Vector::operator=(Vector v)
	{
		size_t size = v.Size();
		DataType *tmp = v._first;
		v._first = _first;
		_first = tmp;
		_finish = _first + size;
		_endofstorage = _finish;
		return *this;
	}
	void Vector::Expand(size_t n)
	{
		DataType *tmp = new DataType[n];
		size_t size = Size();
		memmove(tmp, _first, Size()*sizeof(DataType));
		delete[] _first;
		_first = tmp;
		_finish = _first + size;
		_endofstorage = _first + n;
	}
	void Vector::PushBack(DataType x)
	{
		if (_finish > _endofstorage)
			Expand(2 * Capacity());
		*_finish = x;
		_finish++;
	}
	void Vector::PopBack()
	{
        assert(_first < _finish);
		_finish--;
	}
	void Vector::Insert(size_t pos, DataType x)
    {
        assert(pos<Size());
        if(_finish >= _endofstorage)
            Expand(2*Capacity());
        memmove(_first+pos+1,_first+pos,Size()-pos+1);
        *(_first+pos) = x;
    }
	void Vector::Erase(size_t pos)
    {
        assert(pos<Size());
        memmove(_first+pos,_first+pos+1,(Size()-pos-1)*sizeof(DataType));
        _finish--;
    }
	size_t Vector::Find(DataType x)
    {
        DataType *tmp = _first;
        while(tmp != _finish)
        {
            if(*tmp == x)
                return tmp-_first;
            else
                tmp++;
        }
        return -1;
    }
    //截取n个字符
	void Vector::Reserve(size_t n)
    {
        //如果n<capacity,则什么都不做，将其容量降为size与n之间的最大值
        //会改变capacity,不会改变size，若n>capacity扩容，
        if(n<Capacity())
        {
            _endofstorage = _first + ( n > Size() ? n : Size());
            return;
        }
        else if(n>Capacity())
        {
            Expand(n);
            return;
        }
        else
            return;
    }      
	void Vector::Print()
	{
		DataType *tmp = _first;
		while (tmp != _finish)
		{
			printf("%d ", *tmp);
			tmp++;
		}
		printf("\n");
	}
int main()
{
	Vector v;
	Vector v1(v);
	v.PushBack(1);
	v.PushBack(2);
	v.PushBack(3);
	v.PushBack(4);
	v.PushBack(5);
	v.PushBack(6);
	v.Print();
    v1 = v;
    v1.Print();
    v1.Erase(2);
    v1.Print();
    size_t ret = v1.Find(3);
    printf("%lu\n",ret);
    ret = v1.Find(2);
    printf("%lu\n",ret);
    ret = v1.Find(5);
    printf("%lu\n",ret);
    v1.Reserve(3);
    v1.Print();
	return 0;
}
