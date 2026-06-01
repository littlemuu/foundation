/*
题目：203. Remove Linked List Elements

智能指针练习重点：
- 单链表删除节点时，练习 std::unique_ptr 的所有权转移。
- 删除当前节点时，可以把“拥有当前节点的 unique_ptr”改成当前节点的 next。
- 适合练习“谁拥有这个节点”，而不是只记裸指针。

题目要求：
给定一个链表的头节点 head 和一个整数 val，删除链表中所有满足 Node.val == val 的节点，
并返回新的头节点。

示例 1：
输入：head = [1,2,6,3,4,5,6], val = 6
输出：[1,2,3,4,5]

示例 2：
输入：head = [], val = 1
输出：[]

示例 3：
输入：head = [7,7,7,7], val = 7
输出：[]
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
        /*
        原来写错的版本大概是这样：

            ListNode203* dummy = new ListNode203(0);
            dummy->next = std::move(head);
            ListNode203* cur = dummy->next.get();

            while (cur) {
                if (cur->val == val) dummy->next = std::move(cur->next);
                cur = dummy->next.get();
            }

            while (cur) {
                std::unique_ptr<ListNode203> temp = std::move(cur->next);
                if (temp->val == val) {
                    cur->next = std::move(temp->next);
                }
                cur = cur->next.get();
            }

            return std::move(dummy->next);

        错误关键点：
        1. 如果 dummy->next 的第一个节点不等于 val，第一个 while 会一直回到同一个节点，形成死循环。
        2. 第一个 while 结束时 cur 已经是 nullptr，所以第二个 while 基本不会处理后面的节点。
        3. std::move(cur->next) 会把 cur->next 变成空；如果 temp 不是要删除的节点，也必须接回去，否则链表会断。
        4. dummy 用 new 创建，但没有 delete，会泄漏。这里其实不需要裸指针 dummy。
        5. 每一轮 temp 离开作用域时，如果还拥有节点，会自动释放；所以 unique_ptr 的所有权转移要特别小心。
        
        正确版本：
        std::unique_ptr<ListNode203> removeElements(std::unique_ptr<ListNode203> head, int val) {
            auto dummy = std::make_unique<ListNode203>(0);
            dummy->next = std::move(head);

            ListNode203* cur = dummy.get();

            while (cur->next) {
                if (cur->next->val == val) {
                    cur->next = std::move(cur->next->next);
                } else {
                    cur = cur->next.get();
            }
        }

        return std::move(dummy->next);
        }
        */

        std::unique_ptr<ListNode203>* cur = &head;

        while (*cur) {
            if ((*cur)->val == val) {
                *cur = std::move((*cur)->next);
            } else {
                cur = &((*cur)->next);
            }
        }

        return head;
    }
};
