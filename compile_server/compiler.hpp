#pragma once
#include <iostream>
#include <string>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "../comm/util.hpp"
#include "../comm/log.hpp"

namespace ns_compiler
{
    using namespace ns_util;
    using namespace ns_log;

    class compiler
    {
    public:
        compiler() {}
        ~compiler() {}

    public:
        // 完成文件的编译功能
        // 生成源文件：filename -> filename.cpp
        // 生成可执行文件：filename -> filename.exe
        // 生成标准错误文件：filename -> filename.stderr
        static bool compile(std::string& file_name)
        {
            pid_t pid = fork();
            if (pid < 0) 
            {
                // 创建子进程失败
                LOG(WARING) << "create child process failed" << "\n";
                return false;
            }
            else if (pid == 0)
            {
                // 当g++编译出错时，错误信息会打印到标准错误中
                // 需要将错误信息重定向到 filename.stderr文件中
                int _stderr = open(PathUtil::Stderr(file_name).c_str(), O_CREAT | O_WRONLY, 0644);
                if (_stderr < 0)
                {
                    LOG(ERROR) << "open stderr file failed" << "\n";
                    exit(1);
                }
                dup2(_stderr, 2);

                // 子进程，进行程序替换，执行传入的文件名对应文件里的代码
                // g++ -o dest src -std=c++11
                execlp("g++", "g++", "-o", PathUtil::Exe(file_name).c_str(), \
                PathUtil::Src(file_name).c_str(), "-std=c++11", nullptr);
                LOG(ERROR) << "execlp g++ falied" << "\n";
                exit(2);
            }
            else 
            {
                // 父进程 进行进程等待
                waitpid(pid, nullptr, 0);
                // 进程等待成功，判断是否生成可执行文件
                if (FileUtil::IsFileExits(PathUtil::Exe(file_name)))
                {
                    LOG(INFO) << "compile file success" << "\n";
                    return true;
                }
            }
            LOG(ERROR) << "create file.exe failed" << "\n";
            return false;
        }
    };
}