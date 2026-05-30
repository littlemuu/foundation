/*
题目：133. Clone Graph

智能指针练习重点：
- 图结构可能形成环，适合理解 shared_ptr 的生命周期问题。
- 可以先用 shared_ptr 实现，再思考 neighbors 是否应该改成 weak_ptr。
- 练习用哈希表保存“原节点 -> 克隆节点”的对应关系。

题目要求：
给定无向连通图中的一个节点，返回整个图的深拷贝。
每个节点包含：
1. val：节点编号。
2. neighbors：相邻节点列表。

克隆后的图应满足：
- 节点值相同。
- 邻接关系相同。
- 所有节点都是新创建的节点。

示例：
输入：
adjList = [[2,4],[1,3],[2,4],[1,3]]

含义：
- 1 号节点连接 2 和 4。
- 2 号节点连接 1 和 3。
- 3 号节点连接 2 和 4。
- 4 号节点连接 1 和 3。

输出：
返回克隆图中 1 号节点的指针。
*/

#include <memory>
#include <unordered_map>
#include <vector>

struct GraphNode {
    int val = 0;
    std::vector<std::shared_ptr<GraphNode>> neighbors;

    explicit GraphNode(int x) : val(x) {}
};

class Solution133 {
public:
    std::shared_ptr<GraphNode> cloneGraph(std::shared_ptr<GraphNode> node) {
        return nullptr;
    }
};
