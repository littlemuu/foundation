# List 与迭代器学习笔记

## 解引用与箭头运算符

对于迭代器 `it`：

```cpp
it->mem
(*it).mem
```

这两个表达式的含义相同，都是访问迭代器所指向对象的成员 `mem`。

因此，迭代器中的两个运算符通常分别返回：

```cpp
reference operator*() const {
    return node_ptr->data;
}

pointer operator->() const {
    return &(node_ptr->data);
}
```

## 构造函数初始化列表

错误写法：

```cpp
explicit Iterator(Node<T>* ptr = nullptr) node_ptr(ptr) {
}
```

构造函数的初始化列表前必须有冒号：

```cpp
explicit Iterator(Node<T>* ptr = nullptr) : node_ptr(ptr) {
}
```

`node_ptr(ptr)` 表示使用参数 `ptr` 初始化成员变量 `node_ptr`。

## `this` 与 `*this`

在非静态成员函数中，`this` 是一个指针，指向调用当前函数的对象。

假设有：

```cpp
Iterator<int> it;
++it;
```

在 `it` 的成员函数内部：

```cpp
this   // 指向 it 的指针，类型近似为 Iterator<int>*
*this  // it 对象本身，是一个左值
```

所以：

```cpp
return *this;
```

表示返回当前迭代器对象。

## 前置 `++` 为什么返回引用

前置递增先修改迭代器，然后返回修改后的迭代器本身：

```cpp
self_type& operator++() {
    if (node_ptr) {
        node_ptr = node_ptr->next;
    }
    return *this;
}
```

返回类型是 `self_type&`，因为返回的是当前对象本身，不需要产生副本。这也能支持连续操作：

```cpp
+++it;
```

前置 `--` 的原理相同。

## 后置 `++` 为什么不返回引用

后置递增需要返回递增前的状态，同时把当前迭代器向后移动：

```cpp
self_type operator++(int) {
    self_type temp = *this;
    ++(*this);
    return temp;
}
```

执行过程：

1. 用 `temp` 保存当前迭代器的副本。
2. 使用前置 `++` 修改当前迭代器。
3. 按值返回修改前的副本 `temp`。

不能返回 `temp` 的引用，因为 `temp` 是局部变量，函数结束后会被销毁。

后置 `--` 的写法相同：

```cpp
self_type operator--(int) {
    self_type temp = *this;
    --(*this);
    return temp;
}
```

## 为什么不能写成 `(*this)++`

在后置 `operator++(int)` 内写下面的代码是错误的：

```cpp
self_type operator++(int) {
    return (*this)++;
}
```

`(*this)++` 仍然是后置递增，因此会再次调用当前的 `operator++(int)`。函数不断调用自身，形成无限递归，最终通常会导致栈溢出。

正确做法是调用前置版本：

```cpp
++(*this);  // 正确：调用 operator++()
(*this)++;  // 错误：再次调用 operator++(int)
```

## 前置与后置运算符的声明区别

```cpp
self_type& operator++();    // 前置：++it
self_type operator++(int);  // 后置：it++
```

后置版本中的 `int` 是一个用于区分函数签名的占位参数，通常不会在函数体中使用。

可以记成：

- `++it`：先递增，再返回自己，因此返回引用。
- `it++`：先保存旧值，再递增，最后返回旧值副本，因此按值返回。

## List 实现复查

### `Node*` 和 `iterator` 不能混用

`Node<T>*` 是链表内部使用的节点指针，`iterator` 是提供给使用者的访问工具。当前迭代器可以理解为对节点指针的一层包装：

```cpp
class Iterator {
    Node<T>* node_ptr;
};
```

因此，下面两种类型并不相同：

```cpp
Node<T>* node;
iterator it;
```

从节点指针得到迭代器时，需要显式构造：

```cpp
return iterator(node);
```

由于构造函数带有 `explicit`，不能直接写：

```cpp
return node; // 错误：禁止 Node* 隐式转换为 iterator
```

使用 `explicit` 可以及时发现把内部节点指针和外部迭代器混用的问题。

### 为什么 `end()` 指向尾哨兵

`end()` 表示“最后一个元素之后的位置”，它本身不代表有效元素。因此使用尾哨兵节点作为结束位置：

```cpp
iterator end() {
    return iterator(tail);
}
```

遍历时，在到达尾哨兵前停止：

```cpp
for (auto it = begin(); it != end(); ++it) {
    std::cout << *it << ' ';
}
```

双向迭代器还允许从 `end()` 向前移动到最后一个元素：

```cpp
auto it = end();
--it;
std::cout << *it;
```

### `erase()` 为什么返回下一个迭代器

节点被 `delete` 后，原来的迭代器已经失效，不能继续解引用或移动。因此删除前先保存下一个节点，删除后返回它的迭代器：

```cpp
Node<T>* next = cur->next;
delete cur;
return iterator(next);
```

这使连续删除成为可能：

```cpp
it = list.erase(it);
```

### `push` 和 `pop` 应复用基础操作

`insert()` 已经负责连接节点和增加 `size_`，所以 `push_front()` 和 `push_back()` 不应再次修改指针或数量：

```cpp
void push_front(const T& value) {
    insert(begin(), value);
}

void push_back(const T& value) {
    insert(end(), value);
}
```

同理，`erase()` 已经负责断开节点、释放内存和减少 `size_`。删除头尾元素时只需要找到相应迭代器：

```cpp
void pop_front() {
    if (empty()) return;
    erase(begin());
}

void pop_back() {
    if (empty()) return;
    auto last = end();
    --last;
    erase(last);
}
```

如果调用 `erase()` 后又执行 `--size_`，元素数量会被减少两次。

### `clear()` 和 `delete` 做什么

`new Node<T>` 会在动态内存中创建一个节点；对应的 `delete` 会调用该节点的析构函数并释放这块内存：

```cpp
Node<T>* node = new Node<T>(value);
delete node;
```

`clear()` 应只删除普通数据节点，不能删除 `head` 和 `tail` 两个哨兵。删除后要重新连接哨兵，并把数量归零：

```cpp
head->next = tail;
tail->prev = head;
size_ = 0;
```

析构函数先调用 `clear()` 删除普通节点，再单独释放两个哨兵：

```cpp
~List() {
    clear();
    delete head;
    delete tail;
}
```

### 为什么每个类都要声明模板

`Node`、`Iterator` 和 `List` 是三个彼此独立的类模板。每个模板都拥有自己的模板参数作用域，因此都要分别声明：

```cpp
template <typename T>
struct Node;

template <typename T>
class Iterator;

template <typename T>
class List;
```

提前出现的 `class List;` 只是前置声明，让 `Iterator` 可以把 `List<T>` 声明为友元；它不是后面完整类定义的替代品。

## 本轮测试覆盖

`main()` 使用 `assert` 检查了以下行为：

- 空链表的 `empty()`、`size()`、`begin() == end()`。
- `push_front()`、`push_back()`、`front()` 和 `back()`。
- `insert()` 的位置和返回值。
- 前置 `++`、后置 `++`、从 `end()` 执行 `--`。
- 后置 `--` 返回移动前的位置。
- `operator->` 访问元素成员。
- `erase()` 返回下一个迭代器。
- `pop_front()`、`pop_back()` 和空链表删除。
- `clear()` 后重新插入元素。

`assert(条件)` 要求条件为真；条件为假时程序会立即终止，帮助定位实现错误。

## 当前可以继续完善的地方

当前 `front()` 和 `back()` 默认链表中存在元素。如果对空链表调用它们，会访问哨兵节点中的占位数据；这不应该作为正常用法。

可以选择像标准容器一样，把空链表调用 `front()`、`back()` 视为未定义行为；也可以在练习实现中主动检查并抛出异常：

```cpp
T& front() {
    if (empty()) {
        throw std::logic_error("front() called on empty list");
    }
    return head->next->data;
}
```

`back()` 可以采用相同处理方式。

另外，当前的：

```cpp
using const_iterator = Iterator<T>;
```

并不是真正的只读迭代器，因为 `operator*()` 仍然返回 `T&`，使用者可以通过它修改元素。真正的 `const_iterator` 应返回 `const T&` 和 `const T*`，这可以作为后续练习。
