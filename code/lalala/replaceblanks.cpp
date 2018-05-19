#include <iostream>
class Solution {
public:
    //length : 字符串的容量
	void replaceSpace(char *str,int length) {
    char *str1 = str;
    //空格的个数
	int blankspacecount = 0;
	//原字符串的长度
    int strlen = 0;
	while (*str1 != '\0')
	{
		if (*str1 == ' ')
		{
			blankspacecount++;
		}
		strlen++;
		str1++;
	}
    //求最后的字符串的总长
	int newsize = strlen + 2*blankspacecount;
    //判断是否当前字符串的容量已超出
	if (newsize + 1 > length)
		return;
    //源字符串的末尾
	char *pstr = str + strlen;
    //替换后的字符串的末尾
	char *ppstr = str + newsize;

	while (pstr != ppstr)
	{
		if (*pstr == ' ')
		{
			*ppstr-- = '0';
			*ppstr-- = '2';
			*ppstr-- = '%';
		}
		else
		{
			*ppstr = *pstr;
			--ppstr;
		}
		--pstr;
	}
  	
    }
};
