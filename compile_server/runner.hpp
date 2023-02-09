#pragma once

#include <iostream>
#include <string>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/time.h>
#include <sys/resource.h>

#include "../comm/util.hpp"
#include "../comm/log.hpp"

namespace ns_runner
{
    using namespace ns_log;
    using namespace ns_util;
    class Runner
    {
    public:
        Runner() {}
        ~Runner() {}

    public:
        static void SetProcLimit(int cpu_limit, int mem_limit)
        {
            // 设置cpu时间限制
            struct rlimit cpu_rlimit;
            cpu_rlimit.rlim_max = RLIM_INFINITY;
            cpu_rlimit.rlim_cur = cpu_limit;
            setrlimit(RLIMIT_CPU, &cpu_rlimit);

            // 设置内存限制
            struct rlimit mem_rlimit;
            mem_rlimit.rlim_max = RLIM_INFINITY;
            mem_rlimit.rlim_cur = mem_limit * 1024;
            setrlimit(RLIMIT_AS, &mem_rlimit);
        }

        // 返回值 == 0 : 正常运行完毕
        // 返回值 < 0 :  程序内部错误
        // 返回值 > 0 :  程序异常，收到信号，返回值为信号的值
        // cpu_limit :  程序运行时CPU最大资源上限
        // mem_limit :  程序运行时最大内存限制
        static int Run(const std::string& file_name, int cpu_limit, int mem_limit)
        {
            // 代码运行:
            // 1. 运行成功，结果正确
            // 2. 运行成功，结果错误
            // 3. 运行错误，出现异常
            // 代码结果正确与否: 测试用例决定
            // 代码是否能运行完毕需要进行差错处理
            std::string execute = PathUtil::Exe(file_name);
            std::string stdin_file = PathUtil::Stdin(file_name);
            std::string stdout_file = PathUtil::Stdout(file_name);
            std::string stderr_file = PathUtil::Stderr(file_name);

            // 打开运行时的临时文件
            umask(0);
            int stdin_fd = open(stdin_file.c_str(), O_CREAT|O_RDONLY, 0644);
            int stdout_fd = open(stdout_file.c_str(), O_CREAT|O_WRONLY, 0644);
            int stderr_fd = open(stderr_file.c_str(), O_CREAT|O_WRONLY, 0644);
            if (stdin_fd < 0 || stdout_fd < 0 || stderr_fd < 0)
            {
                // 打开临时文件失败
                LOG(ERROR) << "open running temporary files failed" << "\n";
                return -1;
            }

            pid_t pid = fork();
            if (pid < 0) 
            {
                // 创建子进程失败
                LOG(ERROR) << "process replacement failed" << "\n";
                close(stdin_fd);
                close(stdout_fd);
                close(stderr_fd);
                return -2;
            }
            else if (pid == 0)
            {
                // 子进程，将标准输入，标准输出，标准错误重定向到临时文件中
                dup2(stdin_fd, 0);
                dup2(stdout_fd, 1);
                dup2(stderr_fd, 2);

                // 设置程序CPU运行时长和内存限制
                SetProcLimit(cpu_limit, mem_limit);

                // 程序替换
                execl(execute.c_str(), execute.c_str(), nullptr);
                exit(1);
            }
            else 
            {
                // 父进程
                close(stdin_fd);
                close(stdout_fd);
                close(stderr_fd);
                int status = 0;
                waitpid(pid, &status, 0);
                if (status == 0)
                    LOG(INFO) << "running file success" <<"\n";
                else
                    LOG(ERROR) << "exception occurred " << "code: " << (status & 0x7F) <<"\n";
                // status获取信号
                return status & 0x7F;
            }
        }
    };
}
