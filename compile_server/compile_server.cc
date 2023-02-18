#include "compile_run.hpp"
#include "../comm/httplib.h"

using namespace ns_compile_run;
using namespace httplib;

static void Usage(std::string proc)
{
    std::cout << "Usage: " << proc << " : port" << std::endl;
}

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        Usage(argv[0]);
        return 1;
    }

    Server svr;
    // svr.Get("/hello",[](const Request &req, Response &resp){
    //     // 用来进行基本测试
    //     resp.set_content("hello httplib", "text/plain;charset=utf-8");
    // });

    svr.Post("/compile_and_run", [](const Request& req, Response& resp){
        std::string in_json = req.body;
        std::string out_json;
        if (!in_json.empty())
        {
            CompileAndRun::Start(in_json, &out_json);
            resp.set_content(out_json, "application/json;charset=utf-8");
        } 
    });

    svr.listen("0.0.0.0", atoi(argv[1]));


    // svr.listen("0.0.0.0", 8080);
    // std::string in_json;
    // Json::Value in_value;
    // in_value["code"] = R"(#include <iostream>
    // int main(){
    // std::cout << "hello world" << std::endl;
    // return 0;
    // })";
    // in_value["input"];
    // in_value["cpu_limit"] = 1;
    // in_value["mem_limit"] = 10240 * 10;
    // Json::StyledWriter writer;
    // in_json = writer.write(in_value);
    // std::cout << in_json << std::endl;

    // std::string out_json;
    // CompileAndRun::Start(in_json, &out_json);
    // std::cout << out_json << std::endl;

    return 0;
}