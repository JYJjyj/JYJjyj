#include <iostream>
#include <vector>
#include <stack>
using namespace std;
//利用栈存放链表的值，再依次打印栈顶元素。
struct ListNode {
        int val;
        struct ListNode *next;
        ListNode(int x) :
              val(x), next(NULL) {
        }
  };
class Solution {
public:
    vector<int> printListFromTailToHead(ListNode* head) {
        vector<int> v;
        ListNode* tmp = head;
        stack<int> s;
        while(tmp != NULL)
        {
            s.push(tmp->val);
            tmp = tmp->next;
        }
        while(!s.empty())
        {
            v.push_back(s.top());
            s.pop();
        }
        return v;
    }
};
