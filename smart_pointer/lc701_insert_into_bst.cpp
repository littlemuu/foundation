/*
题目：701. Insert into a Binary Search Tree

智能指针练习重点：
- BST 插入非常适合练习 std::unique_ptr 管理树节点。
- 每个节点独占拥有自己的 left/right 子树。
- 插入时可以递归返回新的子树根，也可以用 unique_ptr<TreeNode>* 作为游标。

题目要求：
给定二叉搜索树的根节点 root 和一个值 val，将 val 插入 BST，并返回插入后的根节点。
题目保证 val 不存在于原 BST 中。

示例 1：
输入：
root = [4,2,7,1,3], val = 5

输出：
[4,2,7,1,3,5]

示例 2：
输入：
root = [40,20,60,10,30,50,70], val = 25

输出：
[40,20,60,10,30,50,70,null,null,25]

示例 3：
输入：
root = [4,2,7,1,3,null,null,null,null,null,null], val = 5

输出：
[4,2,7,1,3,5]
*/

#include <memory>

struct TreeNode701 {
    int val = 0;
    std::unique_ptr<TreeNode701> left;
    std::unique_ptr<TreeNode701> right;

    explicit TreeNode701(int x) : val(x) {}
};

class Solution701 {
public:
    std::unique_ptr<TreeNode701> insertIntoBST(std::unique_ptr<TreeNode701> root, int val) {
        if(root==nullptr){
            root=std::make_unique<TreeNode701>(val);
            return root;
        }
        std::unique_ptr<TreeNode701>* cur=&root;
        while((*cur)){
            if(val<(*cur)->val){
                if((*cur)->left) cur=&((*cur)->left);
                else break;
            }
            else{
                if((*cur)->right) cur=&((*cur)->right);
                else break;
            }
        }
        if((*cur)->val>val) (*cur)->left=std::make_unique<TreeNode701>(val);
        else (*cur)->right=std::make_unique<TreeNode701>(val);
        return root;
    }
};
