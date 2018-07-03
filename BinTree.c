#include "SeqQueue.c"
#include "BinTree.h"

void TreeInit(TreeNode** root)
{
    assert(root);
    assert(*root);
    *root = NULL;
}

void PreOrder(TreeNode* root)
{
    if(root == NULL)
        return;
    printf("%c ",root->data);
    PostOrder(root->lchild);
    PostOrder(root->rchild);
}

void InOrder(TreeNode* root)
{
    if(root == NULL)
        return;
    InOrder(root->lchild);
    printf("%c ",root->data);
    InOrder(root->rchild);
}

void PostOrder(TreeNode* root)
{
  if(root == NULL)
    return;
  PostOrder(root->lchild);
  PostOrder(root->rchild);
  printf("%c ",root->data);
}

void LevelOrder(TreeNode* root)
{
    //层序遍历
    assert(root);
    SeqQueue s;
    SeqQueueInit(&s);
    TreeNode *cur = root;
    SeqQueuePush(&s,cur);
    SeqQueueType tmp;
    while(SeqQueueFront(&s,&tmp))
    {
        printf("%c ",cur->data);
        if(cur->lchild != NULL)
            SeqQueuePush(&s,cur->lchild);
        if(cur->rchild != NULL)
            SeqQueuePush(&s,cur->rchild);
        SeqQueuePop(&s);
        SeqQueueType value;
        SeqQueueFront(&s,&value);
        cur = value; 
    }
  //使用队列。
  //将当前元素入队列，访问队列的首元素，将其左节点与右结点依次入队列
  //将首元素出队列，再执行上一步，直到队列中无元素。
}

// @brief 根据先序遍历结果(带有空节点标记), 
// 构造一棵树 
// @param array[] 先序遍历的结果 
// @param size 数组中元素个数 
// @param null_node 数组中空节点的特殊标记. 
TreeNode *CreateNode(TreeNodeType data)
{
  TreeNode *ret = (TreeNode*)malloc(sizeof(TreeNode));
  ret->rchild = NULL;
  ret->lchild = NULL;
  ret->data = data;
  return ret;
}
TreeNode* _TreeCreate(TreeNodeType array[],size_t size,size_t* index,TreeNodeType null_node)
{
  if(array[*index] == null_node)
    return NULL;
  else
  {
    TreeNode *root = CreateNode(array[*index]);
    (*index)++;
    root->lchild = _TreeCreate(array,size-1,index,null_node);
    (*index)++;
    root->rchild = _TreeCreate(array,size-1,index,null_node);
    return root;
  }
}
TreeNode* TreeCreate(TreeNodeType array[], size_t size, TreeNodeType null_node)
{
  assert(array);
  size_t index = 0;
  return _TreeCreate(array,size,&index,null_node);
}
void DestroyNode(TreeNode *tmp)
{
    assert(tmp);
    free(tmp);
    tmp = NULL;
}
void _TreeDestroy(TreeNode **root)
{
    if(*root == NULL)
        return;
    _TreeDestroy(&(*root)->lchild);
    _TreeDestroy(&(*root)->rchild);
    DestroyNode(*root);
}
void TreeDestroy(TreeNode** root)
{
    assert(root);
    assert(*root);
    _TreeDestroy(root);
    root = NULL;
    *root = NULL;
}

TreeNode* TreeClone(TreeNode* root)
{
    if(root == NULL)
        return NULL;
    TreeNode *newnode = CreateNode(root->data); 
    newnode->lchild = TreeClone(root->lchild);
    newnode->rchild = TreeClone(root->rchild);
    return newnode;
}

size_t TreeSize(TreeNode* root); 


size_t TreeLeafSize(TreeNode* root); 

/** 
 * * @brief 求一棵树第 K 层节点的个数 
 * * 
 * * @param root 
 * * 
 * * @return 
 * */ 
size_t TreeKLevelSize(TreeNode* root, int K); 

size_t TreeHeight(TreeNode* root); 

TreeNode* TreeFind(TreeNode* root, TreeNodeType to_find); 

TreeNode* LChild(TreeNode* node); 

TreeNode* RChild(TreeNode* node); 

TreeNode* Parent(TreeNode* root, TreeNode* node); 

void PreOrderByLoop(TreeNode* root); 

void InOrderByLoop(TreeNode* root); 

void PostOrderByLoop(TreeNode* root); 

void TreeMirror(TreeNode* root); 

int IsCompleteTree(TreeNode* root);
///////////////////////////测试代码///////////////////////////////
#define TESTHEAD printf("------------------------------%s------------------------------------\n",__FUNCTION__)

void testCreateTree()
{
  TreeNodeType array[]="abd##eg###c#f##";
  TreeNode *root = TreeCreate(array,sizeof(array)/sizeof(TreeNodeType),'#');
  printf("root->lchild :%c\n",root->rchild->data);
}
void testPreOrder()
{
    TESTHEAD;
    TreeNodeType array[]="abd##eg###c#f##";
    TreeNode *root = TreeCreate(array,sizeof(array)/sizeof(TreeNodeType),'#');
    PostOrder(root);
    printf("\n");
    InOrder(root);
    printf("\n");
    PreOrder(root);
    printf("\n");
    LevelOrder(root);
    printf("\n");
}
void test()
{
  testCreateTree();
  testPreOrder();
}

int main()
{
  test();
  return 0;
}
