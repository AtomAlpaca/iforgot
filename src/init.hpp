#pragma once
#include "../ollama-hpp/singleheader/ollama.hpp"
#include "system.hpp"
#include <fstream>
#include <filesystem>

std::string getLastLine(std::string str)
{
    std::string res;
    for (int i = str.size() - 1; i >= 0; --i)
    {
        if (str[i] == '\n')
        {
            if (!res.empty())
            {
                std::reverse(res.begin(), res.end());
                return res;
            }
        }
        res.push_back(str[i]);
    }
    return res;
}

bool isCreated(std::string str = "iforgot_model")
{
    auto v = ollama::list_models();
    for (auto s : v)
    {
        if (s == str)
        {
            return true;
        }
    }
    return false;
}

bool isRunning(std::string str = "iforgot_model")
{
    auto v = ollama::list_running_models();
    for (auto s : v)
    {
        if (s == str)
        {
            return true;
        }
    }
    return false;
}

Result<std::vector <std::string>> getHistory()
{
    std::ifstream bashHistory;
    std::vector <std::string> res;
    bashHistory.open(HOME_PATH + "/.bash_history", std::ios::in);
    if (!bashHistory.is_open())
    {
        return Result<std::vector <std::string>>(false, res);
    }
    char buf[1024];
    while (bashHistory.getline(buf, 1023))
    {
        res.push_back(buf);
    }
    return Result<std::vector <std::string>>(true, res);
}

bool createModel(std::vector <std::string> history, std::string model = "qwen3:1.7b")
{
    if (model == "iforgot_model")
    {
        return false;
    }
    if (isCreated())
    {
        if (!ollama::delete_model("iforgot_model"))
        {
            return false;
        }
    }
    if (!isCreated(model))
    {
        if (!ollama::pull_model(model))
        {
            return false;
        }
    }

    std::filesystem::path dirPath = std::filesystem::path(HOME_PATH + "/.iforgot/model_file").parent_path();
    if (!dirPath.empty()) {
    	if (!std::filesystem::exists(dirPath)) {
        	std::filesystem::create_directories(dirPath);
    	}
    }

    auto cli = new httplib::Client("http://localhost:11434");
    nlohmann::json request;
    std::string prompt;

    prompt += "You are a master programmer who is well versed in Linux commands and is familiar with various Linux commands and never makes mistakes. Now you need to provide technical guidance to a newcomer to Linux by correcting the wrong Linux commands provided by him. You need to infer what the most likely command is based on the provided history of Linux commands that have been used, taking into account the user's habits. You don't need to provide the reason for the correction, the process of deduction, or any additional information, just output the most likely correct command.\n";
    auto dis = getDistrib();
    if (dis.isOk())
    {
        prompt += std::string("The user's Linux distribution is ") + dis.unwarp() + "\n";
    }
    prompt += std::string("Here are the Linux commands that the user has used, sorted from oldest to newest in time. Note that there may be illegal commands in the user's history; identify and analyze them very carefully.\n");
    for (auto s : history)
    {
        prompt += s + "\n";
    }

    request["name"] = "iforgot_model";
    request["from"] = model;
    request["system"] = prompt;

    if (auto res = cli -> Post("/api/create", request.dump(), "application/json"))
    {
        nlohmann::json chunk = nlohmann::json::parse(getLastLine(res -> body));
        if (chunk["status"] == "success")
        {
            return true;
        }
    }
    else
    {
        return false;
    }

    return true;
}
