#include <cassert>
#include <cstddef>
#include <iostream>
#include <iterator>
#include <stdexcept>
#include <string>

// ============================================================
// 练习目标：使用环形缓冲区实现一个简化版 Deque，并理解迭代器中的
// “逻辑位置”和元素在缓冲区中的“物理下标”之间的关系。
//
// 建议实现顺序：
// 1. 构造函数、析构函数、empty()、size()
// 2. physical_index()、front()、back()、operator[]
// 3. push_back()、pop_front()，观察下标回绕
// 4. push_front()、pop_back()
// 5. resize() 和 ensure_capacity()
// 6. Iterator 的解引用、移动和比较
// 7. clear()，然后运行全部测试
//
// 当前函数体中的异常只是可编译的占位符，请逐个替换为你的实现。
// ============================================================

template <typename T>
class Deque {
public:
    class Iterator;
    using iterator = Iterator;

    // TODO: 检查 initial_capacity，申请缓冲区并初始化各个下标。
    explicit Deque(std::size_t initial_capacity = 8){
        if(initial_capacity==0) throw std::out_of_range("error");
        capacity_=initial_capacity;
        buffer_=new T[capacity_];
    }

    // TODO: 释放构造函数申请的缓冲区。
    ~Deque() {
        delete [] buffer_;
    }

    // 第一阶段先不处理深拷贝，避免多个 Deque 共同管理同一缓冲区。
    Deque(const Deque& other) = delete;
    Deque& operator=(const Deque& other) = delete;

    // TODO: 判断容器中是否没有元素。
    bool empty() const {
        return count_==0;
    }

    // TODO: 返回当前元素数量，而不是缓冲区容量。
    std::size_t size() const {
        return count_;
    }

    // TODO: 在头部加入元素。缓冲区已满时先扩容。
    void push_front(const T& value) {
        ensure_capacity();
        std::size_t new_front=front_idx_==0?capacity_-1:(front_idx_-1)%capacity_;
        buffer_[new_front]=value;
        front_idx_=new_front;
        count_++;
    }

    // TODO: 在尾部加入元素。back_idx_ 表示下一次尾插的位置。
    void push_back(const T& value) {
        ensure_capacity();
        buffer_[back_idx_] = value;
        back_idx_ = (back_idx_ + 1) % capacity_;
        ++count_;
    }

    // TODO: 删除头部元素。空容器时抛出 std::out_of_range。
    void pop_front() {
        if(count_==0) throw std::out_of_range("out of range");
        front_idx_=(front_idx_+1)%capacity_;
        count_--;
    }

    // TODO: 删除尾部元素。先找到最后一个元素所在的物理下标。
    void pop_back() {
        if(count_==0) throw std::out_of_range("out of range");
        std::size_t new_back=back_idx_ ==0?capacity_ - 1 : back_idx_ - 1;
        back_idx_=new_back;
        count_--;
    }

    // TODO: 返回第一个元素的引用。空容器时抛出 std::out_of_range。
    T& front() {
        if(count_==0) throw std::out_of_range("out of range");
        return buffer_[front_idx_];
    }

    // TODO: 完成 front() 的只读重载。
    const T& front() const {
        if(count_==0) throw std::out_of_range("out of range");
        return buffer_[front_idx_];
    }

    // TODO: 返回最后一个元素的引用。空容器时抛出 std::out_of_range。
    T& back() {
        if(count_==0) throw std::out_of_range("out of range");
        std::size_t index=back_idx_ == 0 ? capacity_ - 1 : back_idx_ - 1;
        return buffer_[index];
    }

    // TODO: 完成 back() 的只读重载。
    const T& back() const {
        if(count_==0) throw std::out_of_range("out of range");
        std::size_t index=back_idx_ == 0 ? capacity_ - 1 : back_idx_ - 1;
        return buffer_[index];
    }

    // TODO: 将逻辑位置 index 转换为物理下标，并返回元素引用。
    // 本练习约定越界时抛出 std::out_of_range。
    T& operator[](std::size_t index) {
        if(index>=count_) throw std::out_of_range("out of range");
        return buffer_[(front_idx_+index)%capacity_];
    }

    // TODO: 完成 operator[] 的只读重载。
    const T& operator[](std::size_t index) const {
        if(index>=count_) throw std::out_of_range("out of range");
        return buffer_[(front_idx_+index)%capacity_];
    }

    // TODO: 清空元素，并把下标恢复到一个一致的空容器状态。
    // 不需要在这里释放缓冲区，清空后应当仍可继续 push。
    void clear() {
        front_idx_=0;
        back_idx_=0;
        count_=0;
    }

    class Iterator {
    public:
        using self_type = Iterator;
        using value_type = T;
        using reference = T&;
        using pointer = T*;
        using iterator_category = std::bidirectional_iterator_tag;
        using difference_type = std::ptrdiff_t;

        // TODO: 保存所属 Deque 的地址和逻辑位置 position。
        explicit Iterator(Deque<T>* deque = nullptr, std::size_t position = 0) {
            deque_ptr_=deque;
            pos_=position;
        }

        // TODO: 把 pos_ 转换为物理下标，返回对应元素的引用。
        reference operator*() const {
            if(pos_>=deque_ptr_->count_) throw std::out_of_range("out of range");
            return deque_ptr_->buffer_[(deque_ptr_->front_idx_+pos_)%deque_ptr_->capacity_];
        }

        // TODO: 返回当前元素的地址。
        pointer operator->() const {
            if(pos_>=deque_ptr_->count_) throw std::out_of_range("out of range");
            return &(deque_ptr_->buffer_[(deque_ptr_->front_idx_+pos_)%deque_ptr_->capacity_]);
        }

        // TODO: 前置 ++，逻辑位置向后移动并返回当前迭代器。
        self_type& operator++() {
            ++pos_;
            return *this;
        }

        // TODO: 后置 ++，返回移动前的迭代器副本。
        self_type operator++(int) {
            Iterator temp=*this;
            ++pos_;
            return temp;
        }

        // TODO: 前置 --，逻辑位置向前移动并返回当前迭代器。
        self_type& operator--() {
            --pos_;
            return *this;
        }

        // TODO: 后置 --，返回移动前的迭代器副本。
        self_type operator--(int) {
            Iterator temp=*this;
            --pos_;
            return temp;
        }

        // TODO: 所属容器和逻辑位置都相同，两个迭代器才相等。
        bool operator==(const self_type& other) const {
            return(deque_ptr_==other.deque_ptr_)&&(pos_==other.pos_);
        }

        // TODO: 可考虑复用 operator==。
        bool operator!=(const self_type& other) const {
            return !operator==(other);
        }

    private:
        Deque<T>* deque_ptr_ = nullptr;
        std::size_t pos_ = 0;  // 相对 front_idx_ 的逻辑位置
    };

    // TODO: 返回逻辑位置 0 的迭代器。
    iterator begin() {
        return Iterator(this,0);
    }

    // TODO: 返回逻辑位置 count_ 的迭代器。
    iterator end() {
        return Iterator(this,count_);
    }

    // TODO: 按逻辑顺序打印元素，用于观察缓冲区回绕后是否仍然有序。
    void print() const {
        for(std::size_t i=0;i<count_;i++){
            std::cout<<buffer_[(front_idx_+i)%capacity_]<<' ';
        }
    }

private:
    // TODO: 将相对队头的逻辑位置转换为 buffer_ 中的物理下标。
    std::size_t physical_index(std::size_t logical_pos) const {
        return (front_idx_+logical_pos)%capacity_;
    }

    // TODO: 申请更大的缓冲区，并按逻辑顺序搬移已有元素。
    // 扩容完成后重新确定 front_idx_ 和 back_idx_。
    void resize(std::size_t new_capacity) {
        T* new_buffer=new T[new_capacity];
        for(std::size_t i=0;i<count_;i++){
            new_buffer[i]=buffer_[(front_idx_+i)%capacity_];
        }
        delete[] buffer_;
        buffer_=new_buffer;
        capacity_=new_capacity;
        front_idx_=0;
        back_idx_=count_;
    }

    // TODO: 当 count_ == capacity_ 时扩容；未满时不做任何事。
    void ensure_capacity() {
        if(count_==capacity_) resize(capacity_*2);
    }

    T* buffer_ = nullptr;
    std::size_t capacity_ = 0;
    std::size_t front_idx_ = 0;  // 第一个元素所在位置
    std::size_t back_idx_ = 0;   // 下一次 push_back 写入的位置
    std::size_t count_ = 0;
};

int main() {
    // 1. 构造和空容器状态
    Deque<int> deque(4);
    assert(deque.empty());
    assert(deque.size() == 0);
    assert(deque.begin() == deque.end());

    // 2. 头尾插入，以及 front()、back()
    deque.push_back(1);
    deque.push_back(2);
    deque.push_front(0);
    assert(!deque.empty());
    assert(deque.size() == 3);
    assert(deque.front() == 0);
    assert(deque.back() == 2);

    // 3. operator[] 使用逻辑下标，不受缓冲区物理布局影响
    assert(deque[0] == 0);
    assert(deque[1] == 1);
    assert(deque[2] == 2);
    deque[1] = 10;
    assert(deque[1] == 10);

    // 4. 使用迭代器从前向后遍历
    int expected[] = {0, 10, 2};
    std::size_t i = 0;
    for (auto it = deque.begin(); it != deque.end(); ++it) {
        assert(*it == expected[i]);
        ++i;
    }
    assert(i == deque.size());

    // 5. 验证前置、后置移动和从 end() 向前移动
    auto it = deque.begin();
    auto old = it++;
    assert(*old == 0);
    assert(*it == 10);

    auto last = deque.end();
    --last;
    assert(*last == 2);
    auto old_last = last--;
    assert(*old_last == 2);
    assert(*last == 10);

    // 6. 头尾删除
    deque.pop_front();
    deque.pop_back();
    assert(deque.size() == 1);
    assert(deque.front() == 10);
    assert(deque.back() == 10);

    // 7. 制造下标回绕，再触发扩容；逻辑顺序必须保持不变
    Deque<int> wrapped(4);
    wrapped.push_back(1);
    wrapped.push_back(2);
    wrapped.push_back(3);
    wrapped.pop_front();
    wrapped.pop_front();
    wrapped.push_back(4);
    wrapped.push_back(5);
    wrapped.push_back(6);
    wrapped.push_back(7);  // 第五个元素触发扩容

    int wrapped_expected[] = {3, 4, 5, 6, 7};
    for (std::size_t index = 0; index < wrapped.size(); ++index) {
        assert(wrapped[index] == wrapped_expected[index]);
    }

    // 8. 越界和空容器操作
    bool caught = false;
    try {
        (void)wrapped[wrapped.size()];
    } catch (const std::out_of_range&) {
        caught = true;
    }
    assert(caught);

    Deque<int> empty_deque;
    caught = false;
    try {
        empty_deque.pop_front();
    } catch (const std::out_of_range&) {
        caught = true;
    }
    assert(caught);

    // 9. clear() 后仍然可以继续使用
    wrapped.clear();
    assert(wrapped.empty());
    wrapped.push_front(8);
    wrapped.push_back(9);
    assert(wrapped.front() == 8);
    assert(wrapped.back() == 9);

    // 10. operator-> 可以访问元素成员
    Deque<std::string> words;
    words.push_back("iterator");
    assert(words.begin()->size() == 8);

    std::cout << "final deque: ";
    wrapped.print();
    std::cout << '\n';
    std::cout << "All tests passed!\n";

    return 0;
}
