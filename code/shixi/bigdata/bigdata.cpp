#include "bigdata.h"
const INT64 maxValue = 0x7fffffffffffffff;
const INT64 minValue = 0x8000000000000000;
BigData::BigData(INT64 value)
    :_value(value)
{}
//模拟实现atoi
BigData::BigData(const string& strData)
    :_value(0)
    ,_strData("+0")
{
//1.判断异常情况
    if(strData.empty())
        return;
    
    char* str = (char*)strData.c_str();
    while(isspace(*str))
        str++;

    if('\0'  == *str)
        return;

    char symbol = str[0];
    if('+' == symbol || '-' == symbol)
        ++str;
    else if(symbol >= '0' && symbol <= '9')
        symbol = '+';
    else
        return;
    
    //跳过前置0
    while('0' == *str)
        ++str;
    if('\0' == *str)
        return ;
//2.处理数字
    
    //数字的第一位是符号位
    _strData.resize(strlen(str) + 1);
    _strData[0] = symbol;

    size_t count = 1;
    while(isdigit(*str))
    {
        _strData[count++] = *str;
        _value = _value * 10 + (*str - '0');        
        ++str;
    }

    //_value可能为负值
    if('-' == symbol)
        _value = 0 - _value;

    _strData.resize(count);
}

ostream& operator<<(ostream& out,const BigData data)
{
//    const char* str = data._strData.c_str();
}

BigData BigData::operator+(const BigData& bg)
{
    if(IsINT64Overflow() && bg.IsINT64Overflow())
    {
        if(_strData[0] != bg._strData[0])
            return BigData(_value + bg._value);
        else
        {
            //同为正 \ 同为负
            if(('+' == _strData[0] && maxValue - _value >= bg._value) ||('-' == _strData[0] && minValue - _value <= bg._value))
            {
                return BigData(_value + bg._value);
            }
        }
        //至少有一个超出long lon的范围
        //加完的结果超出long long的范围
        if(_strData[0] == bg._strData[0])
            BigData(Add(_strData,bg._strData).c_str());
    }
}
