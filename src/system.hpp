#pragma once
#include "result.hpp"
#include <cstring>
#include <string>
#include <iostream>
#include <fstream>

const std::string HOME_PATH = getenv("HOME");;

Result<std::string> getDistrib()
{
    std::ifstream lsbRelease;
    lsbRelease.open("/etc/lsb_release", std::ios::in);
    if (!lsbRelease.is_open())
    {
        return Result<std::string>(false, "");
    }
    char buf[1024];
    while (lsbRelease.getline(buf, 1000))
    {
        std::string line(buf);
        size_t pos = line.find("DISTRIB_ID=");
        if (pos == std::string::npos)
        {
            continue;
        }
        lsbRelease.close();
        std::string res = line.substr(pos + strlen("DISTRIB_ID="));
        if (res.front() == '\"')
        {
            res = res.substr(1);
        }
        if (res.back() == '\"')
        {
            res.pop_back();
        }
        return Result<std::string>(true, res);
    }
    lsbRelease.close();
    return Result<std::string>(false, "");
}
