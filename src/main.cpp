#include "../ollama-hpp/singleheader/ollama.hpp"
#include "arguments.hpp"
#include "init.hpp"
#include <vector>
#include <string>

std::string clean(std::string str)
{
    std::vector <std::string> v;
    std::string tmp, res;
    for (auto c : str)
    {
        if (c == '\n')
        {
            if (!tmp.empty())
            {
                v.push_back(tmp);
                tmp = "";
                continue;
            }
        }
        if (c != '\t' && c != '\n')
        {
            tmp.push_back(c);
        }
    }
    if (!tmp.empty())
    {
        v.push_back(tmp);
    }
    for (auto s : v)
    {
        if (s == "</think>")
        {
            res.clear();
        }
        if (s == "<think>" || s == "</think>")
        {
            continue;
        }
        if (!res.empty())
        {
            res += "\n";
        }
        res += s;
    }
    return res;
}

int main(int argc, char *argv[])
{
    std::vector <std::string> args;
    for (int i = 1; i < argc; ++i)
    {
        args.push_back(argv[i]);
    }
    auto res = parse(args).unwarp();
    if (res.first == HELP)
    {
        std::cout << std::string("\t --help -h: 输出该帮助信息\n") +
                                 "\t --init -i [model name]: 基于 [model name] 初始化模型\n" +
                                 "\t 不带参数运行: 纠正上一个命令\n" +
                                 "\t 带参数运行:   纠正所给出的命令";
    }
    else if (res.first == INIT)
    {
        auto history = getHistory();
        if (!history.isOk())
        {
            std::cerr << "Error: Cannot get bash history";
           return 1;
        }
        if (!res.second.empty())
        {
            if (!createModel(history.unwarp(), res.second.front()))
            {
                std::cerr << "Error: Cannot init model";
            }
        }
        else
        {
            if (!createModel(history.unwarp()))
            {
                std::cerr << "Error: Cannot init model";
            }
        }
    }
    else if (res.first == FIX)
    {
        std::cout << "I am thinking...\n";
        std::cout << clean(ollama::generate("iforgot_model", "Please help me to correct error linux commands. Please answer the question succinctly and directly output the command that is most likely to be correct, you are not allowed to output any additional information! Please fix this command: `" + res.second.front() + "`"));
        std::cout << std::endl;
    }
}
