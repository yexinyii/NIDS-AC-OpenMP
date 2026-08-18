#ifndef AC_AUTOMATON_H
#define AC_AUTOMATON_H

#include <string>
#include <vector>
#include <unordered_set>

class AcAutomaton {
public:
    AcAutomaton();
    ~AcAutomaton() = default;

    // 拷贝语义 — vector<Node> 自动深拷贝，无需手动管理
    AcAutomaton(const AcAutomaton& other) = default;
    AcAutomaton& operator=(const AcAutomaton& other) = default;

    // 移动语义 — 降低并行线程间复制开销
    AcAutomaton(AcAutomaton&& other) noexcept = default;
    AcAutomaton& operator=(AcAutomaton&& other) noexcept = default;
    void build_ac_automaton(std::string singleRule, std::string tag);

    // 值拷贝 clone，供 OpenMP 线程生成独立只读副本
    AcAutomaton clone() const;

    // 插入单条攻击特征（同一终点可绑定多条标签）
    void insert(const std::string& pattern, const std::string& tag);

    // 构建失配指针（全部特征插入后调用一次，重复调用无副作用）
    void build();

    // 查询文本中命中的攻击标签集合（去重），const 保证只读线程安全
    std::unordered_set<std::string> query(const std::string& text) const;

    // 是否已完成 fail 构建
    bool is_built() const { return built; }

    // 清空重置自动机，支持重新加载规则
    void clear();

private:
    static constexpr int ASCII_SIZE = 128;
    static constexpr int INVALID    = -1;

    // 下标式节点：全部存储在 vector<Node> 连续内存中
    struct Node {
        int  next[ASCII_SIZE];             // 子节点下标，INVALID=-1 表示无
        int  fail;                         // 失配指针下标
        std::vector<std::string> tags;     // 命中该终点时触发的攻击标签

        Node() : fail(INVALID) {
            for (int i = 0; i < ASCII_SIZE; ++i) next[i] = INVALID;
        }
    };

    std::vector<Node> nodes;   // nodes[0] 固定为根节点
    bool built = false;

    int create_node();           // 追加节点并返回下标
};

#endif
