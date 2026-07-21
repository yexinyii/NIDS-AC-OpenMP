#include "ac_automaton.h"
#include <queue>
#include <stdexcept>
#include <unordered_set>


//  构造 — nodes[0] 即为根节点，根 fail 始终指向自身


ACAutomaton::ACAutomaton()
{
    createNode();   // 预置根节点于下标 0
    nodes[0].fail = 0;
}


//  clone — 值返回，依赖默认拷贝构造
// （vector<Node> 自动深拷贝）

ACAutomaton ACAutomaton::clone() const
{
    return ACAutomaton(*this);
}

 
//  createNode — 追加节点至连续内存，返回下标


int ACAutomaton::createNode()
{
    nodes.emplace_back();
    return static_cast<int>(nodes.size()) - 1;
}

void ACAutomaton::build_ac_automaton(std::string singleRule, std::string tag){
    insert(singleRule, tag);
}

//  insert — 将攻击特征逐字符插入Trie，终点追加标签


void ACAutomaton::insert(const std::string& pattern, const std::string& tag)
{
    if (pattern.empty()) return;  // 空模式串直接跳过，防止根节点误打标签

    int cur = 0;   // 从根开始
    for (unsigned char c : pattern) {
        if (c >= ASCII_SIZE) continue;           // 跳过非ASCII高字节
        if (nodes[cur].next[c] == INVALID) {
            nodes[cur].next[c] = createNode();   // 不可用引用——createNode()会使vector扩容失效引用
        }
        cur = nodes[cur].next[c];
    }
    nodes[cur].tags.push_back(tag);              // 终点追加标签
    built_ = false;                              // 插入新规则后标记为未构建
}


//  build — BFS构建失配指针 + 路径压缩（查询一步跳转，无循环回退）


void ACAutomaton::build()
{
    if (built_) return;           // 幂等保护

    std::queue<int> q;

    // 初始化根节点子节点：不存在的直接指回根
    for (int i = 0; i < ASCII_SIZE; ++i) {
        int child = nodes[0].next[i];
        if (child != INVALID) {
            nodes[child].fail = 0;
            q.push(child);
        } else {
            nodes[0].next[i] = 0;  // 路径压缩：根节点缺失的子节点指向自身
        }
    }

    // BFS逐层构建 fail + 补全缺失路径
    while (!q.empty()) {
        int cur = q.front();
        q.pop();

        for (int i = 0; i < ASCII_SIZE; ++i) {
            int child = nodes[cur].next[i];
            if (child != INVALID) {
                // 真实存在的子节点：fail = 父节点 fail 对应位置的子节点
                nodes[child].fail = nodes[nodes[cur].fail].next[i];
                q.push(child);
            } else {
                // 不存在的子节点：直接复用 fail 节点的对应路径
                nodes[cur].next[i] = nodes[nodes[cur].fail].next[i];
            }
        }
    }

    built_ = true;
}


//  query — 多模式匹配，返回去重标签集合（const只读，线程安全）


std::unordered_set<std::string> ACAutomaton::query(const std::string& text) const
{
    if (!built_) {
        throw std::runtime_error("ACAutomaton::query() called before build()");
    }

    std::unordered_set<std::string> results;
    int cur = 0;   // 从根开始

    for (unsigned char c : text) {
        if (c >= ASCII_SIZE) {
            cur = 0;         // 非ASCII字符时状态复位
            continue;
        }

        // 路径压缩后：一步跳转，无需循环沿 fail 链回退
        cur = nodes[cur].next[c];

        // 沿 fail 链向上收集所有命中标签
        for (int temp = cur; temp != 0; temp = nodes[temp].fail) {
            for (const std::string& tag : nodes[temp].tags) {
                results.insert(tag);
            }
        }
    }

    return results;
}

//  clear — 清空所有节点与状态，支持重新加载规则


void ACAutomaton::clear()
{
    nodes.clear();
    nodes.emplace_back();   // 重建根节点 nodes[0]
    nodes[0].fail = 0;
    built_ = false;
}
