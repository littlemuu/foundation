/*
题目：450. Delete Node in a BST

智能指针练习重点：
- 二叉搜索树删除节点时，需要移动整棵子树的所有权。
- 推荐使用 std::unique_ptr<TreeNode> 表示 left/right。
- 重点练习删除有两个子节点的节点：可以找右子树最小节点或左子树最大节点替换。

题目要求：
给定一个二叉搜索树的根节点 root 和一个值 key，删除 BST 中值等于 key 的节点。
删除后仍要保持二叉搜索树性质，并返回新的根节点。

示例 1：
输入：
root = [5,3,6,2,4,null,7], key = 3

输出：
[5,4,6,2,null,null,7]

说明：
3 被删除后，可以用 4 替代它。其他合法 BST 结果也可以。

示例 2：
输入：
root = [5,3,6,2,4,null,7], key = 0

输出：
[5,3,6,2,4,null,7]

示例 3：
输入：
root = [], key = 0

输出：
[]
*/

#include <memory>

struct TreeNode450 {
    int val = 0;
    std::unique_ptr<TreeNode450> left;
    std::unique_ptr<TreeNode450> right;

    explicit TreeNode450(int x) : val(x) {}
};

class Solution450 {
public:
    std::unique_ptr<TreeNode450> deleteNode(std::unique_ptr<TreeNode450> root, int key) {
        return root;
    }
};
