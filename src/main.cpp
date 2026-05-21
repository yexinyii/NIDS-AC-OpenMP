#include "file_io.h"
#include "preprocess.h"
#include "ac_automaton.h"

#include <iostream>
#include <vector>
#include <string>

using namespace std;

int main()
{
    string rulePath = "rules/rules.txt";
    vector<string> rules;

    // 读取攻击规则库
    try
    {
        rules = FileIO::readLines(rulePath);
        cout << "读取规则库成功，共 " << rules.size() << " 条规则" << endl;
    }
    catch (const std::exception &e)
    {
        cerr << "读取规则库失败: " << e.what() << endl;
        return 1;
    }

    // 构建AC自动机
    build_ac_automaton(rules);

    // HTTP测试文件列表
    vector<string> filePaths;
    filePaths.push_back("dataset/http_extracted/http_1.txt");
    filePaths.push_back("dataset/http_extracted/http_2.txt");
    filePaths.push_back("dataset/http_extracted/http_3.txt");
    filePaths.push_back("dataset/http_extracted/http_4.txt");
    filePaths.push_back("dataset/http_extracted/http_5.txt");

    for (int i = 0; i < filePaths.size(); i++)
    {
        string filePath = filePaths[i];
        cout << endl;
        cout << "正在处理：" << filePath << endl;

        try
        {
            vector<string> dataLines = FileIO::readLines(filePath);

            string content = "";
            for (int j = 0; j < dataLines.size(); j++)
            {
                content += dataLines[j] + "\n";
            }

            // 预处理后匹配
            ac_automaton_match(to_lowercase(content));
        }
        catch (const std::exception &e)
        {
            cerr << "处理文件失败: " << e.what() << endl;
        }
    }

    // system("pause");
    return 0;
}
