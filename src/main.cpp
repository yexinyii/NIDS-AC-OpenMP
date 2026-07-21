#include "file_io.h"
#include "preprocess.h"
#include "ac_automaton.h"

#include <iostream>
#include <vector>
#include <string>

int main()
{
    std::string rulePath = "rules/rules.txt";
    std::vector<std::string> rules;
    ACAutomaton* acAutoaton = new ACAutomaton;

    // 读取攻击规则库
    try
    {
        rules = FileIO::readLines(rulePath);
        std::cout << "读取规则库成功，共 " << rules.size() << " 条规则" << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cerr << "读取规则库失败: " << e.what() << std::endl;
        return 1;
    }
    
    // 构建AC自动机
    for(auto &i : rules){
        char tagTarget = '|';
        int tagTargetIndex = i.find(tagTarget);
        std::string tag = i.substr(0, tagTargetIndex-1);
        acAutoaton->build_ac_automaton(i, tag);
    }
    // HTTP测试文件列表
    std::vector<std::string> filePaths;
    filePaths.push_back("dataset/http_extracted/http_1.txt");
    filePaths.push_back("dataset/http_extracted/http_2.txt");
    filePaths.push_back("dataset/http_extracted/http_3.txt");
    filePaths.push_back("dataset/http_extracted/http_4.txt");
    filePaths.push_back("dataset/http_extracted/http_5.txt");

    for (int i = 0; i < filePaths.size(); i++)
    {
        std::string filePath = filePaths[i];
        std::cout << std::endl;
        std::cout << "正在处理：" << filePath << std::endl;

        try
        {
            std::vector<std::string> dataLines = FileIO::readLines(filePath);

            std::string content = "";
            for (int j = 0; j < dataLines.size(); j++)
            {
                content += dataLines[j] + "\n";
            }

            // 预处理后匹配
            acAutoaton->build();
            std::unordered_set<std::string> ac_automaton_matched = acAutoaton->query(to_lowercase(content));
        }
        catch (const std::exception &e)
        {
            std::cerr << "处理文件失败: " << e.what() << std::endl;
        }
    }
    return 0;
}
