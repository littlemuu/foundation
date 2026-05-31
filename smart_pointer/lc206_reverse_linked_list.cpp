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
#include <iostream>

struct ListNode206 {
    int val = 0;
    std::unique_ptr<ListNode206> next;

    explicit ListNode206(int x) : val(x) {}
};

class Solution206 {
public:
    std::unique_ptr<ListNode206> reverseList(std::unique_ptr<ListNode206> head) {
        /*
        原来写错的版本大概是这样：

            if (!head || !head->next) return head;
            std::unique_ptr<ListNode206> new_head = std::move(head->next);
            while (new_head) {
                head->next = std::move(new_head->next);
                new_head->next = std::move(head);
                head = std::move(new_head);
                new_head = std::move(head->next->next);
            }
            return head;

        问题在最后一行：
            new_head = std::move(head->next->next);

        这时 head 已经变成了新的反转头节点，head->next 已经不是原链表里的
        “下一个节点”，而是反转后接在 head 后面的旧节点。再从
        head->next->next 取节点，会把原链表后面的所有权链条弄断，所以
        [1,2,3,4,5] 最后只会输出 5 4。
        */
        std::unique_ptr<ListNode206> new_head = nullptr;

        while (head) {
            std::unique_ptr<ListNode206> next = std::move(head->next);
            head->next = std::move(new_head);
            new_head = std::move(head);
            head = std::move(next);
        }

        return new_head;
    }
};

void print(const ListNode206* head){
    const ListNode206* cur=head;
    while(cur){
        std::cout<<cur->val<<" ";
        cur=cur->next.get();    
    }
    std::cout<<std::endl;
}

int main(){
    std::unique_ptr<ListNode206> dummy(new ListNode206(0));
    for(int i=5;i>=1;i--){
        std::unique_ptr<ListNode206> temp(new ListNode206(i));
        temp->next=std::move(dummy->next);
        dummy->next=std::move(temp);  //这一步之后temp就是空的了
    }
    std::cout<<"before reverse: ";
    print(dummy->next.get());
    Solution206 sol;
    std::unique_ptr<ListNode206> result=sol.reverseList(std::move(dummy->next));
    std::cout<<"after reverse: ";
    print(result.get());
    return 0;
}
