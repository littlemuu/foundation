#include <cstddef>
#include <iterator>
#include <stdexcept>
#include <iostream>
#include <cassert>
#include <string>

// ============================================================
// 练习目标：使用双向链表理解迭代器，并实现一个简化版 List。
//
// 建议实现顺序：
// 1. Node
// 2. Iterator 的解引用、移动和比较
// 3. List 的构造、析构、begin、end、empty
// 4. insert、erase
// 5. 其余成员函数
// ============================================================

// 链表节点：保存数据，以及前后节点的地址。
template <typename T>
struct Node {
    T data;
    Node* prev;
    Node* next;

    // TODO: 初始化 data、prev 和 next。
    //传进来一个 T 类型的值，我只读它，不改它，并且尽量不复制它。
    explicit Node(const T& value = T()):data(value),prev(nullptr),next(nullptr) {
    }
};

// Iterator 中需要把 List 声明为友元，因此先声明 List。
template <typename T>
class List;

// 双向迭代器：内部保存当前节点的地址。
template <typename T>
class Iterator {
public:
    using self_type = Iterator<T>;
    using value_type = T;
    using reference = T&;
    using pointer = T*;
    using iterator_category = std::bidirectional_iterator_tag;
    using difference_type = std::ptrdiff_t;

    // TODO: 保存传入的节点地址。
    explicit Iterator(Node<T>* ptr = nullptr):node_ptr(ptr){ 
    }

    // TODO: 返回当前节点中数据的引用。
    reference operator*() const {
        return node_ptr->data;
    }

    // TODO: 返回当前节点中数据的地址。
    pointer operator->() const {
        return &(node_ptr->data);
    }

    // TODO: 前置 ++，移动到后一个节点，并返回当前迭代器。
    self_type& operator++() {
        if(node_ptr) node_ptr=node_ptr->next;
        return *this;       //this是什么？为什么这里的函数返回值有&但是下一个函数没有？
    }

    // TODO: 后置 ++，返回移动前的迭代器。
    self_type operator++(int) {
        self_type temp=*this;
        ++(*this);
        return temp;
    }

    // TODO: 前置 --，移动到前一个节点，并返回当前迭代器。
    self_type& operator--() {
        if(node_ptr) node_ptr=node_ptr->prev;
        return *this;
    }

    // TODO: 后置 --，返回移动前的迭代器。
    self_type operator--(int) {
        self_type temp=*this;
        --(*this);
        return temp;
    }

    // TODO: 判断两个迭代器是否指向同一个节点。
    bool operator==(const self_type& other) const {
        return node_ptr==other.node_ptr;
    }

    // TODO: 判断两个迭代器是否指向不同节点。
    bool operator!=(const self_type& other) const {
        return node_ptr!=other.node_ptr;
    }

private:
    Node<T>* node_ptr = nullptr;

    // List 需要访问 node_ptr，以完成插入和删除。
    friend class List<T>;
};

// 简化版双向链表。
// 设计提示：可以使用 head 和 tail 两个哨兵节点简化边界处理。
template <typename T>  //为什么要反复定义模板呢？
class List {
public:
    using iterator = Iterator<T>;
    using const_iterator = Iterator<T>;  // TODO: 进阶练习可实现真正的只读迭代器。

    // TODO: 创建头、尾哨兵节点，并正确连接它们。
    List() {
        head=new Node<T>();
        tail=new Node<T>();
        head->next=tail;
        tail->prev=head;
    }

    // TODO: 释放所有普通节点和哨兵节点。
    ~List() {
        clear();    //这个函数做了什么？
        delete head;
        delete tail;   //delete做什么？
    }

    // 第一阶段先不处理深拷贝，避免多个 List 共同管理同一批节点。
    List(const List& other) = delete;
    List& operator=(const List& other) = delete;

    // TODO: 在 pos 指向的节点之前插入 value，返回新节点的迭代器。
    iterator insert(iterator pos, const T& value) {
        Node<T>* cur=pos.node_ptr;
        Node<T>* cur_prev=pos.node_ptr->prev;
        Node<T>* new_node=new Node<T>(value);
        cur_prev->next=new_node;
        new_node->next=cur;
        cur->prev=new_node;
        new_node->prev=cur_prev;
        size_++;
        return iterator(new_node);  //和直接返回new_node有什么区别？到底为什么要用迭代器呢？
    }

    // TODO: 删除 pos 指向的节点，返回下一个节点的迭代器。
    iterator erase(iterator pos) {
        Node<T>* cur=pos.node_ptr;
        if(cur==head||cur==tail) throw std::logic_error("illegal!");
        Node<T>* next=cur->next;
        cur->prev->next=cur->next;
        cur->next->prev=cur->prev;
        delete cur;
        --size_;
        return iterator(next);
    }

    // TODO: 在链表头部插入元素，可考虑复用 insert。
    void push_front(const T& value) {
        insert(begin(),value);
    }

    // TODO: 在链表尾部插入元素，可考虑复用 insert。
    void push_back(const T& value) {
        insert(end(),value);
    }

    // TODO: 删除第一个元素；空链表时不要删除哨兵节点。
    void pop_front() {
        if(empty()) return;
        erase(begin());
    }

    // TODO: 删除最后一个元素；空链表时不要删除哨兵节点。
    void pop_back() {
        if(empty()) return;
        iterator last=end();
        --last;
        erase(last);
    }

    // TODO: 返回第一个元素的引用。
    T& front() {
        return head->next->data;
    }

    // TODO: 返回最后一个元素的引用。
    T& back() {
        return tail->prev->data;
    }

    // TODO: 判断链表是否为空。
    bool empty() const {
        return head->next==tail;
    }

    // TODO: 统计元素数量。可以遍历，也可以增加 size 成员变量。
    std::size_t size() const {
        return size_;
    }

    // TODO: 删除全部普通节点，并恢复空链表状态。
    void clear() {
        Node<T>* cur=head->next;
        while(cur!=tail){
            Node<T>* temp=cur;
            cur=cur->next;
            delete temp;
        }
        head->next=tail;
        tail->prev=head;
        size_=0;
    }

    // TODO: 返回指向第一个元素的迭代器。
    iterator begin() {
        return iterator(head->next);
    }

    // TODO: 返回表示遍历结束位置的迭代器。
    iterator end() {
        return iterator(tail);
    }

    // TODO: 遍历并打印链表，用于检查实现结果。
    void print() const {
        Node<T>* cur=head->next;
        while(cur!=tail){
            std::cout<<cur->data<<' ';
            cur=cur->next;
        }
    }

private:
    Node<T>* head = nullptr;  // 头哨兵节点
    Node<T>* tail = nullptr;  // 尾哨兵节点

    std::size_t size_=0;
};

int main() {
    List<int> list;

    // 1. 空链表
    assert(list.empty());
    assert(list.size()==0);
    assert(list.begin()==list.end());

    // 2. 头尾插入，以及 front()、back()
    list.push_front(1);
    list.push_back(3);
    assert(!list.empty());
    assert(list.size()==2);
    assert(list.front()==1);
    assert(list.back()==3);

    // 3. 在 3 前插入 2，此时链表应为 1 2 3
    auto pos=list.begin();
    ++pos;
    auto inserted=list.insert(pos,2);
    assert(*inserted==2);
    assert(list.size()==3);

    // 4. 使用迭代器从前向后遍历
    int expected=1;
    for(auto it=list.begin();it!=list.end();++it){
        assert(*it==expected);
        ++expected;
    }

    // 5. 验证后置 ++ 返回移动前的迭代器
    auto it=list.begin();
    auto old=it++;
    assert(*old==1);
    assert(*it==2);

    // 6. 从 end() 向前移动，可以取得最后一个元素
    auto last=list.end();
    --last;
    assert(*last==3);

    // 后置 -- 返回移动前的位置，last 自身则移动到前一个元素
    auto old_last=last--;
    assert(*old_last==3);
    assert(*last==2);

    // 7. erase() 返回被删除节点的下一个位置
    auto next=list.erase(inserted);
    assert(*next==3);
    assert(list.size()==2);

    // 8. 删除头尾元素
    list.pop_front();
    assert(list.front()==3);
    list.pop_back();
    assert(list.empty());
    assert(list.size()==0);

    // 空链表调用 pop 不应出错
    list.pop_front();
    list.pop_back();

    // 9. clear() 后仍然可以继续使用
    list.push_back(4);
    list.push_back(5);
    list.clear();
    assert(list.empty());
    list.push_back(6);
    assert(list.front()==6);
    assert(list.back()==6);

    // 10. operator-> 可以直接访问元素的成员
    List<std::string> words;
    words.push_back("iterator");
    assert(words.begin()->size()==8);

    std::cout<<"final list: ";
    list.print();
    std::cout<<'\n';
    std::cout<<"All tests passed!\n";

    return 0;
}
