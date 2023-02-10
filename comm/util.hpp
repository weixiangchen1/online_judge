#pragma once

#include <iostream>
#include <string>
#include <atomic>
#include <fstream>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/time.h>
#include <boost/algorithm/string.hpp>

namespace ns_util
{
    class TimeUtil
    {
    public:
        static std::string GetTimeStamp()
        {
            struct timeval tv;
            gettimeofday(&tv, nullptr);
            return std::to_string(tv.tv_sec);
        }
        // 获取毫秒级别时间戳
        static std::string GetTimeMs()
        {
            struct timeval tv;
            gettimeofday(&tv, nullptr);
            return std::to_string(tv.tv_sec * 1000 + tv.tv_usec / 1000);
        }
    };

    const std::string temp_path = "./temp/";
    class PathUtil
    {
    public:
        static std::string AddSuffix(const std::string& file_name, const std::string& suffix)
        {
            std::string path_name = temp_path;
            path_name += file_name;
            path_name += suffix;
            return path_name;
        }

        // 编译时文件
        // 拼接文件名：filename -> filename.cpp
        static std::string Src(const std::string& file_name)
        {
            return AddSuffix(file_name, ".cpp");
        }
        // 拼接文件名：filename -> filename.exe
        static std::string Exe(const std::string& file_name)
        {
            return AddSuffix(file_name, ".exe");
        }
        // 拼接文件名：filename -> filename.stderr
        static std::string Compile_stderr(const std::string& file_name)
        {
            return AddSuffix(file_name, ".complie_stderr");
        }

        // 运行时文件
        static std::string Stdin(const std::string& file_name)
        {
            return AddSuffix(file_name, ".stdin");
        }
        static std::string Stdout(const std::string& file_name)
        {
            return AddSuffix(file_name, ".stdout");
        }
        static std::string Stderr(const std::string& file_name)
        {
            return AddSuffix(file_name, ".stderr");
        }

    };

    class FileUtil
    {
    public:
        static bool IsFileExists(const std::string& file_name)
        {
            struct stat st;
            if (stat(file_name.c_str(), &st) == 0) 
            {
                // 获取文件属性成功，代表文件存在
                return true;
            }
            return false;
        }

        static std::string UniqueFileName()
        {
            static std::atomic_uint id(0);
            id++;
            std::string ms = TimeUtil::GetTimeMs();
            std::string unique_id = std::to_string(id);
            return ms + "_" + unique_id;
        }

        static bool WriteFile(const std::string& target, const std::string& content)
        {
            std::ofstream out(target);
            if (!out.is_open())
            {
                return false;
            }
            out.write(content.c_str(), content.size());
            out.close();
            return true;
        }

        static bool ReadFile(const std::string& target, std::string* content, bool keep)
        {
            (*content).clear();
            std::ifstream in(target);
            if (!in.is_open())
            {
                return false;
            }
            std::string line;
            while (std::getline(in, line))
            {
                (*content) += line;
                (*content) += (keep ? "\n" : "");
            } 

            in.close();
            return true;
        }
    };

    class StringUtil
    {
    public:
        static void SplitString(const std::string& target, std::vector<std::string>* out, const std::string& sep)
        {
            boost::split(*out, target, boost::is_any_of(sep), boost::algorithm::token_compress_on);
        }
    };
}