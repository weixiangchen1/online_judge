#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <cassert>
#include <unordered_map>

#include "../comm/util.hpp"
#include "../comm/log.hpp"

namespace ns_model
{
    using namespace ns_log;
    using namespace ns_util;

    struct Question
    {
        std::string number;         // 题目编号
        std::string title;          // 题目标题
        std::string level;          // 题目难度
        int cpu_limit;              // 时间复杂度限制
        int mem_limit;              // 空间复杂度限制
        std::string desc;           // 题目描述
        std::string header;         // 题目预设的代码
        std::string tail;           // 题目的测试用例
    };

    const std::string question_list = "./questions/questions.list";
    const std::string question_path = "./questions/";

    class Model
    {
    private:
        std::unordered_map<std::string, Question> questions;
    public:
        Model() 
        {
            assert(LoadQuestionList(question_list));
        }

        // 加载 题目列表 和 所有题目信息
        bool LoadQuestionList(const std::string& question_list)
        {
            std::ifstream in(question_list);
            if (!in.is_open())
            {
                LOG(FATAL) << "Load question list failed" << "\n";
                return false;
            }

            std::string line;
            while (std::getline(in, line))
            {
                std::vector<std::string> out;
                StringUtil::SplitString(line, &out, " ");  
                // 1 回文数 简单  1 50000    
                if (out.size() != 5) 
                {
                    LOG(WARING) << "Load part of questions failed" << "\n";
                    continue;
                }

                Question q;
                q.number = out[0];
                q.title = out[1];
                q.level = out[2];
                q.cpu_limit = atoi(out[3].c_str());
                q.mem_limit = atoi(out[4].c_str()); 

                std::string path = question_path;
                path += q.number;
                path += "/";

                FileUtil::ReadFile(path+"desc.txt", &q.desc, true);
                FileUtil::ReadFile(path+"header.cpp", &q.header, true);
                FileUtil::ReadFile(path+"tail.cpp", &q.tail, true);

                questions.insert({q.number, q});
            }
            LOG(INFO) << "Load question list success" << "\n";
            in.close();
        }

        // 获取所有题目列表
        bool GetAllQuestions(std::vector<struct Question>* out)
        {
            if (questions.size() == 0) 
            {
                LOG(ERROR) << "User Get all question failed" << "\n";
                return false;
            }

            for (const auto& q : questions) 
            {
                (*out).push_back(q.second);
            }
            return true;
        }

        // 获取某一题目详情
        bool GetOneQuestion(const std::string& number, Question* q)
        {
            const auto& it = questions.find(number);
            if (it == questions.end())
            {
                LOG(ERROR) << "User get No." << q->number << " question failed" << "\n";
                return false;
            }

            *q = it->second;
            return true;
        }

        ~Model() {}
    };
}