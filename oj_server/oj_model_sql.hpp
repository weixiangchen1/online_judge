#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <cassert>
#include <unordered_map>

#include "./include/mysql.h"
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
        std::string desc;           // 题目描述
        std::string header;         // 题目预设的代码
        std::string tail;           // 题目的测试用例
        int cpu_limit;              // 时间复杂度限制
        int mem_limit;              // 空间复杂度限制
    };

    const std::string oj_questions = "oj_questions";
    const std::string host = "127.0.0.1";
    const std::string user = "oj_client";
    const std::string passwd = "AsDfGhJk123~!@";
    const std::string db = "oj";
    const int port = 3306;

    class Model
    {
    public:
        Model() {}

        bool QueryMySQL(std::string& sql, std::vector<Question>* out)
        {
            // 1. 创建句柄
            MYSQL* my = mysql_init(nullptr);
            // 2. 连接数据库
            if (nullptr == mysql_real_connect(my, host.c_str(), user.c_str(), passwd.c_str(), db.c_str(), port, nullptr, 0))
            {
                LOG(FATAL) << "Connect database failed" << "\n";
                return false;
            }
            mysql_set_character_set(my, "utf8");
            LOG(INFO) << "Connect database success" << "\n";
            // 3. 执行mysql命令
            if (0 != mysql_query(my, sql.c_str()))
            {
                LOG(ERROR) << "Execute " << sql << " failed" << "\n";
                return false;
            }
            // 4. 提取结果
            MYSQL_RES* result = mysql_store_result(my);
            int rows = mysql_num_rows(result); 
            // 5. 分析结果
            struct Question q;
            for (int i = 0; i < rows; ++i)
            {
                MYSQL_ROW line = mysql_fetch_row(result);
                q.number = line[0];
                q.title = line[1];
                q.level = line[2];
                q.desc = line[3];
                q.header = line[4];
                q.tail = line[5];
                q.cpu_limit = atoi(line[6]);
                q.mem_limit = atoi(line[7]);

                out->push_back(q);
            }
            // 6. 关闭数据库
            free(result);
            mysql_close(my);
            return true;
        }

        // 获取所有题目列表
        bool GetAllQuestions(std::vector<struct Question>* out)
        {
            std::string sql = "select * from ";
            sql += oj_questions;
            return QueryMySQL(sql, out);
        }

        // 获取某一题目详情
        bool GetOneQuestion(const std::string& number, Question* q)
        {
            std::string sql = "select * from ";
            sql += oj_questions;
            sql += " where number=";
            sql += number;
            
            bool ret = false;
            std::vector<struct Question> out;
            if (QueryMySQL(sql, &out))
            {
                if (out.size() == 1)
                {
                    *q = out[0];
                    ret = true;
                }
            }
            return ret;
        }

        ~Model() {}
    };
}