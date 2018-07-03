#include <iostream>
#include <stdlib.h>
using namespace std;

void testatoi()
{
    cout << atoi("\0")<<endl;
}
int main()
{
    testatoi();
    return 0;
}
