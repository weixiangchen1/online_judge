#include <iostream>
#include <string>

#include "../comm/httplib.h"

using namespace httplib;

int main()
{
    // 用户请求进行路由的服务
    Server svr;

    // 1. 用户请求获取题目列表
    svr.Get("/all_questions", [](const Request& req, Response& resp){
        resp.set_content("This is question list", "text/plain;charset=utf-8");
    });

    // 2. 用户请求题目详情
    svr.Get(R"(/question/(\d+))", [](const Request& req, Response& resp){
        std::string number = req.matches[1];
        resp.set_content("user request question desc: " + number, "text/plain;charset=utf-8");
    });

    // 3. 用户请求题目编译运行
    svr.Get(R"(/judge/(\d+))", [](const Request& req, Response& resp){
        std::string number = req.matches[1];
        resp.set_content("user request question compile and run: " + number, "text/plain;charset=utf-8");
    });

    svr.set_base_dir("./wwwroot");
    svr.listen("0.0.0.0", 8888);

    return 0;
}