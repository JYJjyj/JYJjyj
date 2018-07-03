//模拟atoi
//12345
//0000000000000000012345
//+1234
//-12
//+00000123
//-000024 
//+               390573
//-              894
//""
//      2434
//      4545jrdfs243
//suyd29310
//
#pragma once
#include <iostream>
#include <string>
#include <string.h>
using namespace std;

typedef long long INT64;

class BigData
{
    public:
        BigData(INT64 value = 0);
        BigData(const string& strData);
        friend ostream& operator<<(ostream& out,const BigData& data);

        BigData operator+(const BigData& bg);
        BigData operator-(const BigData& bg);
        BigData operator*(const BigData& bg);
        BigData operator/(const BigData& bg);
    private:
        bool IsINT64Overflow() const
        {
            string _strTemp("+9223372036854775807");//八个字节能表示的最大数字
            if('-' == _strData[0])
            {
                _strTemp = "-9223372036854775808";
            }

            size_t LSize = _strData.size();
            size_t RSize = _strTemp.size();
            if(LSize < RSize)
            {
                return true;
            }
            else if(LSize == RSize && _strData < _strTemp)
                return false;
        }
        INT64 _value;
        string _strData;
};
