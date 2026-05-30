/*
题目：206. Reverse Linked List

智能指针练习重点：
- 反转链表非常适合练习 std::unique_ptr 的移动语义。
- 每次循环需要保存 next，再把当前节点的 next 指向已经反转好的前缀。
- 注意 unique_ptr 不能拷贝，只能 std::move。

题目要求：
给定单链表的头节点 head，反转链表，并返回反转后的链表头节点。

示例 1：
输入：
head = [1,2,3,4,5]

输出：
[5,4,3,2,1]

示例 2：
输入：
head = [1,2]

输出：
[2,1]

示例 3：
输入：
head = []

输出：
[]
*/

#include <memory>

struct ListNode206 {
    int val = 0;
    std::unique_ptr<ListNode206> next;

    explicit ListNode206(int x) : val(x) {}
};

class Solution206 {
public:
    std::unique_ptr<ListNode206> reverseList(std::unique_ptr<ListNode206> head) {
        return head;
    }
};
