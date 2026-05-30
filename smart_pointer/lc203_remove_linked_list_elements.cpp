/*
题目：203. Remove Linked List Elements

智能指针练习重点：
- 单链表删除节点时，练习 std::unique_ptr 的所有权转移。
- 删除当前节点时，可以把当前节点的拥有者指针改为当前节点的 next。
- 适合练习“谁拥有这个节点”而不是只记裸指针。

题目要求：
给定一个链表的头节点 head 和一个整数 val，删除链表中所有满足 Node.val == val 的节点，
并返回新的头节点。

示例 1：
输入：
head = [1,2,6,3,4,5,6], val = 6

输出：
[1,2,3,4,5]

示例 2：
输入：
head = [], val = 1

输出：
[]

示例 3：
输入：
head = [7,7,7,7], val = 7

输出：
[]
*/

#include <memory>

struct ListNode203 {
    int val = 0;
    std::unique_ptr<ListNode203> next;

    explicit ListNode203(int x) : val(x) {}
};

class Solution203 {
public:
    std::unique_ptr<ListNode203> removeElements(std::unique_ptr<ListNode203> head, int val) {
        return head;
    }
};
