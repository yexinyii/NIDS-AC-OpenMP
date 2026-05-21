#ifndef AC_AUTOMATON_H
#define AC_AUTOMATON_H

#include <string>
#include <vector>

// 构建AC自动机
void build_ac_automaton(const std::vector<std::string>& patterns);

// AC自动机多模式匹配
void ac_automaton_match(const std::string& text);

#endif
