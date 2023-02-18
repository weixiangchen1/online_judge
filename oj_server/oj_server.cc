#include <iostream>
#include <string>
#include <signal.h>

#include "./oj_control.hpp"
#include "../comm/httplib.h"

using namespace httplib;
using namespace ns_control;

static Control* ctrl_ptr = nullptr;

void Recovery(int signo)
{
    ctrl_ptr->RecoveryMachines();
}

int main()
{
    // 注册捕捉信号函数
    signal(SIGQUIT, Recovery);

    // 用户请求进行路由的服务
    Server svr;
    Control ctrl;
    ctrl_ptr = &ctrl;

    // 1. 用户请求获取题目列表
    svr.Get("/all_questions", [&ctrl](const Request& req, Response& resp){
        std::string html;
        ctrl.AllQuestions(&html);
        resp.set_content(html, "text/html;charset=utf-8");
    });

    // 2. 用户请求题目详情
    svr.Get(R"(/question/(\d+))", [&ctrl](const Request& req, Response& resp){
        std::string number = req.matches[1];
        std::string html;
        ctrl.Question(number, &html);
        resp.set_content(html, "text/html;charset=utf-8");
    });

    // 3. 用户请求题目编译运行
    svr.Post(R"(/judge/(\d+))", [&ctrl](const Request& req, Response& resp){
        std::string number = req.matches[1];
        std::string result_json;
        ctrl.Judge(number, req.body, &result_json);
        resp.set_content(result_json, "application/json;charset=utf-8");
    });

    svr.set_base_dir("./wwwroot");
    svr.listen("0.0.0.0", 8888);

    return 0;
}
