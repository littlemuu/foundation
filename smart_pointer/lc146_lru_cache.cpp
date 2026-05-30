/*
题目：146. LRU Cache

题目要求：
设计一个 LRU 缓存，支持两个操作：
1. get(key)：如果 key 存在，返回对应 value，并把该 key 标记为最近使用；否则返回 -1。
2. put(key, value)：如果 key 已存在，更新 value 并标记为最近使用；
   如果 key 不存在，插入新键值对。若插入后超过容量，移除最久未使用的键。

要求：
- get 和 put 的平均时间复杂度应为 O(1)。

示例：
LRUCache cache(2);
cache.put(1, 1);
cache.put(2, 2);
cache.get(1);    // 返回 1
cache.put(3, 3); // 淘汰 key 2
cache.get(2);    // 返回 -1
cache.put(4, 4); // 淘汰 key 1
cache.get(1);    // 返回 -1
cache.get(3);    // 返回 3
cache.get(4);    // 返回 4

本文件约定：
- dummy->next 是最近使用的节点。
- dummy->prev 是最久未使用的节点。
- unordered_map 保存 key -> Node*，用于 O(1) 定位节点。
*/

#include <iostream>
#include <unordered_map>

struct Node {
    int key;
    int val;
    Node* next;
    Node* prev;

    Node(int k, int v) : key(k), val(v), next(nullptr), prev(nullptr) {}
};

class LRUCache {
public:
    explicit LRUCache(int capacity) : capacity_(capacity), count_(0), dummy_(new Node(-1, -1)) {
        dummy_->next = dummy_;
        dummy_->prev = dummy_;
    }

    ~LRUCache() {
        Node* cur = dummy_->next;
        while (cur != dummy_) {
            Node* next = cur->next;
            delete cur;
            cur = next;
        }
        delete dummy_;
    }

    int get(int key) {
        auto it = umap_.find(key);
        if (it == umap_.end()) {
            return -1;
        }

        Node* cur = it->second;
        moveToHead(cur);
        return cur->val;
    }

    void put(int key, int value) {
        if (capacity_ <= 0) {
            return;
        }

        auto it = umap_.find(key);
        if (it != umap_.end()) {
            Node* cur = it->second;
            cur->val = value;
            moveToHead(cur);
            return;
        }

        Node* cur = new Node(key, value);
        addToHead(cur);
        umap_[key] = cur;
        ++count_;

        if (count_ > capacity_) {
            Node* old = removeTail();

            /*
            之前这里容易写错：
                umap.erase(capacity_);

            capacity_ 只是容量，不是要删除的 key。淘汰链表节点时，
            必须用被淘汰节点自己保存的 key 删除 map。
            */
            umap_.erase(old->key);

            delete old;
            --count_;
        }
    }

private:
    void removeNode(Node* node) {
        node->prev->next = node->next;
        node->next->prev = node->prev;
    }

    void addToHead(Node* node) {
        node->prev = dummy_;
        node->next = dummy_->next;
        dummy_->next->prev = node;
        dummy_->next = node;
    }

    void moveToHead(Node* node) {
        removeNode(node);

        /*
        之前 get 里移动节点时，核心问题是只改了一部分指针，例如：
            cur->next = dumb->next;
            dumb->next = cur;
            dumb->next->prev = cur;
            cur->prev = dumb;

        这段里面 dumb->next 已经被改成 cur 了，再写 dumb->next->prev = cur，
        实际改到的是 cur->prev，而不是旧头节点的 prev。
        正确做法是按 addToHead 的顺序，先让 node 连接旧头节点，再让旧头节点指回 node。
        */
        addToHead(node);
    }

    Node* removeTail() {
        Node* old = dummy_->prev;
        removeNode(old);
        old->next = nullptr;
        old->prev = nullptr;
        return old;
    }

    int capacity_;
    int count_;
    std::unordered_map<int, Node*> umap_;
    Node* dummy_;
};

int main() {
    LRUCache cache(2);
    cache.put(1, 1);
    cache.put(2, 2);
    std::cout << cache.get(1) << std::endl; // 1
    cache.put(3, 3);
    std::cout << cache.get(2) << std::endl; // -1
    cache.put(4, 4);
    std::cout << cache.get(1) << std::endl; // -1
    std::cout << cache.get(3) << std::endl; // 3
    std::cout << cache.get(4) << std::endl; // 4
    return 0;
}
