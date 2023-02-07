#pragma once

#include <iostream>
#include <string>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/time.h>

namespace ns_util
{
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
        static std::string Stderr(const std::string& file_name)
        {
            return AddSuffix(file_name, ".stderr");
        }
    };

    class FileUtil
    {
    public:
        static bool IsFileExits(const std::string& file_name)
        {
            struct stat st;
            if (stat(file_name.c_str(), &st) == 0) 
            {
                // 获取文件属性成功，代表文件存在
                return true;
            }
            return false;
        }
    };

    class TimeUtil
    {
    public:
        static std::string GetTimeStamp()
        {
            struct timeval tv;
            gettimeofday(&tv, nullptr);
            return std::to_string(tv.tv_sec);
        }
    };
}