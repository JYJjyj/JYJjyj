#include <iostream>
#include <algorithm>
#include <stddef.h>
#include <vector>
using namespace std;
class Solution {
public:
    int FindGreatestSumOfSubArray(vector<int> array) {
        size_t  size = array.size();
        int maxsum = array[0];
        int sum = 0;
        size_t i = 0;
        for(;i<size;i++)
        {
            if(sum < 0)
                sum = 0;
            sum += array[i];
            maxsum = max(sum,maxsum);
        }
    return maxsum;
    }
};
