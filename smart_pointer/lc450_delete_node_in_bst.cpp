/*
题目：450. Delete Node in a BST

智能指针练习重点：
- 二叉搜索树删除节点时，需要移动整棵子树的所有权。
- 推荐使用 std::unique_ptr<TreeNode> 表示 left/right。
- 重点练习删除有两个子节点的节点：可以找右子树最小节点或左子树最大节点替换。

题目要求：
给定一个二叉搜索树的根节点 root 和一个值 key，删除 BST 中值等于 key 的节点。
删除后仍要保持二叉搜索树性质，并返回新的根节点。

示例：
输入：
root = [5,3,6,2,4,null,7], key = 3

输出：
[5,4,6,2,null,null,7]

说明：
3 被删除后，可以用 4 替代它。其他合法 BST 结果也可以。
*/

#include <iostream>
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
        if (root == nullptr) {
            return nullptr;
        }

        if (key < root->val) {
            root->left = deleteNode(std::move(root->left), key);
            return root;
        }

        if (key > root->val) {
            root->right = deleteNode(std::move(root->right), key);
            return root;
        }

        if (root->left == nullptr) {
            return std::move(root->right);
        }

        if (root->right == nullptr) {
            return std::move(root->left);
        }

        /*
        你原来这里写错的核心片段是：

            TreeNode450* pre = nullptr;
            std::unique_ptr<TreeNode450> cur = std::move(root->right);
            while (cur->left != nullptr) {
                pre = cur.get();
                cur = std::move(cur->left);
            }
            if (pre != nullptr) pre->left = std::move(cur->right);

        问题：
        1. cur 是 unique_ptr，拥有当前子树。
        2. cur = std::move(cur->left) 会破坏 cur 原来拥有的父节点。
        3. pre 是指向这个父节点的裸指针，父节点被释放后 pre 就变成悬空指针。
        4. root->right 一开始已经被 move 走，后面再接回 root->right 时容易丢子树。

        正确思路：
        不直接移动“当前节点 cur”，而是保存“拥有当前节点的 unique_ptr 的地址”。
        这样找到后继节点后，可以安全地把它从原位置拆下来。
        */
        std::unique_ptr<TreeNode450>* successor_owner = &root->right;
        while ((*successor_owner)->left != nullptr) {
            successor_owner = &((*successor_owner)->left);
        }

        std::unique_ptr<TreeNode450> successor = std::move(*successor_owner);
        *successor_owner = std::move(successor->right);

        successor->left = std::move(root->left);
        successor->right = std::move(root->right);
        return successor;
    }
};

void printInOrder(const TreeNode450* root) {
    if (root == nullptr) {
        return;
    }

    printInOrder(root->left.get());
    std::cout << root->val << " ";
    printInOrder(root->right.get());
}

int main() {
    std::unique_ptr<TreeNode450> root(new TreeNode450(5));
    root->left.reset(new TreeNode450(3));
    root->right.reset(new TreeNode450(6));
    root->left->left.reset(new TreeNode450(2));
    root->left->right.reset(new TreeNode450(4));
    root->right->right.reset(new TreeNode450(7));

    std::cout << "before delete: ";
    printInOrder(root.get());
    std::cout << std::endl;

    Solution450 sol;
    root = sol.deleteNode(std::move(root), 3);

    std::cout << "after delete 3: ";
    printInOrder(root.get());
    std::cout << std::endl;

    return 0;
}
