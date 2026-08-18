#include "file_io.h"
#include "preprocess.h"
#include "ac_automaton.h"

#include <iostream>
#include <vector>
#include <string>

int main() {
    std::string rulePath = "rules/rules.txt";
    std::vector<std::string> rules;
    AcAutomaton acAutomaton;

    // 读取攻击规则库
    try {
        rules = FileIo::read_lines(rulePath);
        std::cout << "读取规则库成功，共 " << rules.size() << " 条规则" << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "读取规则库失败: " << e.what() << std::endl;
        return 1;
    }
    
    // 构建AC自动机
    for (auto& rule : rules) {
        char tagTarget = '|';
        int tagTargetIndex = rule.find(tagTarget);
        std::string tag = rule.substr(0, tagTargetIndex - 1);
        acAutomaton.build_ac_automaton(rule, tag);
    }
    // HTTP测试文件列表
    std::vector<std::string> filePaths;
    filePaths.push_back("dataset/http_extracted/http_1.txt");
    filePaths.push_back("dataset/http_extracted/http_2.txt");
    filePaths.push_back("dataset/http_extracted/http_3.txt");
    filePaths.push_back("dataset/http_extracted/http_4.txt");
    filePaths.push_back("dataset/http_extracted/http_5.txt");

    for (const auto& filePath : filePaths) {
        std::cout << std::endl;
        std::cout << "正在处理：" << filePath << std::endl;

        try {
            std::vector<std::string> dataLines = FileIo::read_lines(filePath);

            std::string content = "";
            for (std::size_t j = 0; j < dataLines.size(); ++j) {
                content += dataLines[j] + "\n";
            }

            // 预处理后匹配
            acAutomaton.build();
            std::unordered_set<std::string> acAutomatonMatched = acAutomaton.query(to_lowercase(content));
        } catch (const std::exception& e) {
            std::cerr << "处理文件失败: " << e.what() << std::endl;
        }
    }
    return 0;
}
