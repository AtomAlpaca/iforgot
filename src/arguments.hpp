#pragma once
#include "result.hpp"
#include <vector>
#include <string>

enum ARG_TYPE
{
    HELP,
    INIT,
    FIX
};

Result<std::pair <int, std::vector <std::string>>> parse(std::vector <std::string> args)
{
    typedef Result<std::pair <int, std::vector <std::string>>> PARSE_RES;
    std::string command;
    std::vector <std::string> res;
    if (!args.size())
    {
        return PARSE_RES(false, std::make_pair(FIX, res));
    }
    for (int i = 0; i < args.size(); ++i)
    {
        if (args[i] == "--help" || args[i] == "-h")
        {
            return PARSE_RES(true, std::make_pair(HELP, res));
        }
        if (args[i] == "--init" || args[i] == "-i")
        {
            if (i + 1 < args.size())
            {
                res.push_back(args[i + 1]);
            }
            return PARSE_RES(true, std::make_pair(INIT, res));
        }
        command = command + " " + args[i];
    }
    res.push_back(command);
    return PARSE_RES(true, std::make_pair(FIX, res));
}
