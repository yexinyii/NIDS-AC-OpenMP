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

    // 读取攻击规则库
    try
    {
        rules = FileIO::readLines(rulePath);

        std::cout << "读取规则库成功，共 "
                  << rules.size()
                  << " 条规则" << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cerr << "读取规则库失败: "
                  << e.what()
                  << std::endl;
        return 1;
    }

    // 构建AC自动机
    build_ac_automaton(rules);

    // HTTP测试文件列表
    std::vector<std::string> filePaths = {
        "dataset/http_extracted/http_1.txt",
        "dataset/http_extracted/http_2.txt",
        "dataset/http_extracted/http_3.txt",
        "dataset/http_extracted/http_4.txt",
        "dataset/http_extracted/http_5.txt"
    };

    for (const auto& filePath : filePaths)
    {
        std::cout << std::endl;
        std::cout << "正在处理：" << filePath << std::endl;

        try
        {
            // 读取HTTP文本内容
            std::vector<std::string> dataLines =
                FileIO::readLines(filePath);

            std::string content;

            for (const auto& line : dataLines)
            {
                content += line + "\n";
            }

            // 预处理：统一转小写
            std::string processed =
                to_lowercase(content);

            // AC自动机匹配检测
            ac_automaton_match(processed);
        }
        catch (const std::exception &e)
        {
            std::cerr << "处理文件失败: "
                      << e.what()
                      << std::endl;
        }
    }

    return 0;
}
