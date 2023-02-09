#pragma once

#include<jsoncpp/json/json.h>

#include "compiler.hpp"
#include "runner.hpp"
#include "../comm/log.hpp"
#include "../comm/util.hpp"

namespace ns_compile_run
{
    using namespace ns_compiler;
    using namespace ns_runner;
    using namespace ns_log;
    using namespace ns_util;

    class CompileAndRun
    {
    public:
        static void RemoveTempFile(const std::string& file_name)
        {
            std::string stdin_file = PathUtil::Stdin(file_name);
            if (FileUtil::IsFileExists(stdin_file)) unlink(stdin_file.c_str());

            std::string stdout_file = PathUtil::Stdout(file_name);
            if (FileUtil::IsFileExists(stdout_file)) unlink(stdout_file.c_str());

            std::string stderr_file = PathUtil::Stderr(file_name);
            if (FileUtil::IsFileExists(stderr_file)) unlink(stderr_file.c_str());

            std::string exe_file = PathUtil::Exe(file_name);
            if (FileUtil::IsFileExists(exe_file)) unlink(exe_file.c_str());

            std::string src_file = PathUtil::Src(file_name);
            if (FileUtil::IsFileExists(src_file)) unlink(src_file.c_str());

            std::string complie_file = PathUtil::Compile_stderr(file_name);
            if (FileUtil::IsFileExists(complie_file)) unlink(complie_file.c_str());
        }
        // 返回错误码对应的描述信息
        // code == 0  程序正常运行结束
        // code < 0   程序运行内部错误
        // code > 0   程序收到信号，异常终止
        static std::string CodeToDesc(int code, const std::string& file_name)
        {
            std::string desc;
            switch (code)
            {
            case 0:
                desc = "Code complie and run success";
                break;
            case -1:
                desc = "User send code is empty";
                break;
            case -2:
                desc = "Unknown error";
                break;
            case -3:
                FileUtil::ReadFile(PathUtil::Compile_stderr(file_name), &desc, true);
                break;
            case SIGFPE: // 8号信号
                desc = "Divide by 0 errors";
                break;
            case SIGABRT:
                desc = "Memory limit exceeded";
                break;
            case SIGXCPU:
                desc = "CPU time limit exceeded";
                break;
            default:
                desc = "Unknown: " + std::to_string(code);
                break;
            }

            return desc;
        }
        // 输入: 
        // code: 用户提交的代码   input: 代码的输入数据  cpu_limit: 最大时间限制  mem_limit: 最大空间限制
        // 输出: 
        // 必填: status: 状态码    reason: 请求结果
        // 选填: stdout: 程序运行完的结果  stderr: 程序运行完的错误结果  
        // in_json:  用户传入的序列化字符串
        // out_json: 需要构建给上层的序列化字符串
        static void Start(const std::string& in_json, std::string* out_json)
        {
            // in_json 反序列化
            Json::Value in_value;
            Json::Reader reader;
            reader.parse(in_json, in_value);

            // 获取输入数据
            std::string code = in_value["code"].asString();
            std::string input = in_value["input"].asString();
            int cpu_limit = in_value["cpu_limit"].asInt();
            int mem_limit = in_value["mem_limit"].asInt();

            Json::Value out_value;
            int status_code = 0;
            int run_result = 0;
            std::string file_name;

            if (code.size() == 0) 
            {
                // 差错处理 + 构造序列化字符串返回
                status_code = -1;  // 代码为空
                goto END;
            }

            file_name = FileUtil::UniqueFileName();

            if (!FileUtil::WriteFile(PathUtil::Src(file_name), code))
            {
                status_code = -2;  // 未知错误
                goto END;
            }

            if (!compiler::compile(file_name))
            {
                status_code = -3;  // 编译出错
                goto END;
            }

            run_result = Runner::Run(file_name, cpu_limit, mem_limit);
            if (run_result < 0)
            {
                status_code = -2;  // 未知错误
            }
            else if (run_result > 0)
            {               
                status_code = run_result;  // 程序崩溃  
            }
            else
            {
                status_code = 0;
            }
        END:   // 遇到对应错误，跳转代码到此处，无需进行错误之后的操作
            out_value["status"] = status_code;
            out_value["reason"] = CodeToDesc(status_code, file_name);
            if (status_code == 0) 
            {
                std::string stdout_str;
                FileUtil::ReadFile(PathUtil::Stdout(file_name), &stdout_str, true);
                out_value["stdout"] = stdout_str;

                std::string stderr_str;
                FileUtil::ReadFile(PathUtil::Stderr(file_name), &stderr_str, true);
                out_value["stderr"] = stderr_str;
            }

            // 序列化
            Json::StyledWriter writer;
            *out_json = writer.write(out_value);

            // 删除temp目录下的临时文件
            RemoveTempFile(file_name);
        }
    };
}

