#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
#include <cassert>
#include <mutex>
#include <fstream>
#include <jsoncpp/json/json.h>

// #include "./oj_model.hpp"
#include "./oj_model_sql.hpp"
#include "./oj_view.hpp"
#include "../comm/log.hpp"
#include "../comm/util.hpp"
#include "../comm/httplib.h"

namespace ns_control
{
    using namespace ns_log;
    using namespace ns_util;
    using namespace ns_model;
    using namespace ns_view;
    using namespace httplib;

    // 提供编译运行的服务机器信息
    class Machine
    {
    public:
        std::string ip;        // 提供服务的机器的IP地址     
        int port;              // 提供服务的机器的端口号
        uint64_t load;         // 提供服务的机器的负载情况
        std::mutex* mtx;       // mutex锁保护load资源
    public:
        Machine()
            :ip(""), port(0), load(0), mtx(nullptr)
        {}
        ~Machine() {}
    public:
        // 增加主机负载
        void IncLoad()
        {
            if (mtx) mtx->lock();
            ++load;
            if (mtx) mtx->unlock();
        }
        // 减少主机负载
        void DecLoad()
        {
            if (mtx) mtx->lock();
            --load;
            if (mtx) mtx->unlock();
        }
        // 重置主机负载
        void RecoveryLoad()
        {
            if (mtx) mtx->lock();
            load = 0;
            if (mtx) mtx->unlock();
        }
        // 显示主机负载
        uint64_t Load()
        {
            uint64_t cur_load = 0;
            if (mtx) mtx->lock();
            cur_load = load;
            if (mtx) mtx->unlock();
            return cur_load;
        }
    };

    const std::string server_machine = "./conf/server_machine.conf";
    // 负载均衡模块
    class LoadBalance
    {
    private:
        // 提供服务的所有机器，数组下标表示主机id
        std::vector<Machine> machines;
        // 所有在线的机器id
        std::vector<int> online;
        // 所有离线的机器id
        std::vector<int> offline;
        std::mutex mtx;
    public:
        LoadBalance() 
        {
            assert(LoadConf(server_machine));
            LOG(INFO) << "Load machines success" << "\n";
        }
        ~LoadBalance() {}
    public:
        // 加载文件中的机器信息
        bool LoadConf(const std::string& machine_conf)
        {
            std::ifstream in(machine_conf);
            if (!in.is_open())
            {
                LOG(FATAL) << "Load machine conf failed" << "\n";
                return false;
            }

            std::string line;
            while (std::getline(in, line))
            {
                std::vector<std::string> tokens;
                StringUtil::SplitString(line, &tokens, ":");
                if (tokens.size() != 2)
                {
                    LOG(WARNING) << "load " << line << " failed" << "\n";
                    continue;
                }

                Machine m;
                m.ip = tokens[0];
                m.port = atoi(tokens[1].c_str());
                m.load = 0;
                m.mtx = new std::mutex();

                online.push_back(machines.size());
                machines.push_back(m);
            }

            in.close();
            return true;
        }
        // 智能选择主机进行服务
        // id: 输出型参数   m: 输出型参数
        bool SmartChoice(int* id, Machine** m)
        {   
            // 1. 使用智能选择的主机，更新主机负载
            // 2. 有可能离线主机
            mtx.lock();
            int online_num = online.size();
            if (online_num == 0)
            {
                mtx.unlock();
                LOG(FATAL) << "All machines is offline" << "\n";
                return false;
            }

            // 循环遍历找到负载最小的机器
            uint64_t min_load = machines[online[0]].Load();
            *m = &machines[online[0]];
            *id = online[0];
            for (int i = 1; i < online_num; ++i)
            {
                u_int64_t cur_load = machines[i].Load();
                if (cur_load < min_load)
                {
                    min_load = cur_load;
                    *m = &machines[online[i]];
                    *id = online[i];
                }
            }
            
            mtx.unlock();
            return true;
        }
        // 下线某台主机
        void OfflineMachine(int which)
        {
            mtx.lock();
            for (auto iter = online.begin(); iter != online.end(); iter++)
            {
                if (*iter == which)
                {
                    machines[which].RecoveryLoad();
                    offline.push_back(*iter);
                    online.erase(iter);
                    break;
                }
            }
            mtx.unlock();
        }
        // 一键上线所有主机
        void OnlineMachine()
        {
            mtx.lock();
            // 将所有离线主机添加到online
            online.insert(online.end(), offline.begin(), offline.end());
            // 将offline清空
            offline.erase(offline.begin(), offline.end());
            mtx.unlock();
        }
        // 显示在线和离线主机
        void ShowMachines()
        {
            std::cout << "online machines: " << std::endl;
            for (auto& it : online)
            {
                std::cout << it << " "; 
            }
            std::cout << std::endl;
            std::cout << "offline machines: " << std::endl;
            for (auto& it : offline)
            {
                std::cout << it << " "; 
            }
            std::cout << std::endl;
        }
    };

    // 业务逻辑的控制器模块
   class Control
   {
    private:
        Model _model;               // 提供后台数据
        View _view;                 // 提供视图功能
        LoadBalance _load_balance;  // 负载均衡处理器
    public:
        Control() {}
        ~Control() {}
    public:
        // 一键上线所有机器
        void RecoveryMachines()
        {
            _load_balance.OnlineMachine();
            LOG(INFO) << "All machines is online now" << "\n";
        }

        // 获取所有题目信息
        bool AllQuestions(std::string* html)
        {
            bool ret = true;
            std::vector<struct Question> all;   
            if (_model.GetAllQuestions(&all))
            {
                sort(all.begin(), all.end(), [](const struct Question& q1, const struct Question& q2){
                    return atoi(q1.number.c_str()) < atoi(q2.number.c_str());
                });
                _view.AllExpandHtml(all, html);
            }
            else
            {
                *html = "Get all questions failed";
                ret = false;
            }
            return ret;
        }

        // 获取特定题目信息
        bool Question(const std::string number, std::string* html)
        {
            bool ret = true;
            struct Question q;
            if (_model.GetOneQuestion(number, &q))
            {
                _view.OneExpandHtml(q, html);
            }
            else
            {
                *html = "Get No." + number + " question failed";
                ret = false;
            }
            return ret;
        }

        // 判题功能
        void Judge(const std::string& number, const std::string& in_json, std::string* out_json)
        {
            // 1. 根据题目编号，获取题目详情
            struct Question q;
            _model.GetOneQuestion(number, &q);
            // 2. 对in_json进行反序列化
            Json::Reader reader;
            Json::Value in_value;
            reader.parse(in_json, in_value);
            std::string code = in_value["code"].asString();

            // 3. 拼接用户代码和测试用例代码
            Json::Value compile_value;
            compile_value["input"] = in_value["input"].asString();
            compile_value["code"] = code + "\n" + q.tail;
            compile_value["cpu_limit"] = q.cpu_limit;
            compile_value["mem_limit"] = q.mem_limit;
            Json::FastWriter writer;
            std::string compile_string = writer.write(compile_value);

            // 4. 负载均衡选择
            while (true)
            {
                int id = 0;
                Machine* m = nullptr;
                if (!_load_balance.SmartChoice(&id, &m))
                {
                    break;
                }
                // 5. 构建http请求，得到运行结果
                Client cli(m->ip, m->port);
                m->IncLoad();
                LOG(INFO) << "Smart choice machine success, machine id: " << id << " machine: " << m->ip << ":" << m->port << " load: " << m->Load() << "\n";
                if (auto res = cli.Post("/compile_and_run", compile_string, "application/json;charset=utf-8"))
                {
                    if (res->status == 200)
                    {
                        // 6. 将结果赋值out_json
                        *out_json = res->body;
                        m->DecLoad();
                        LOG(INFO) << "Request compile and run server success" << "\n";
                        break;
                    }
                    m->DecLoad();
                }
                else
                {
                    // 负载均衡选择主机失败
                    LOG(ERROR) << "Smart choice failed, machine id: " << id << " machine: " << m->ip << ":" << m->port << " will offline" << "\n";
                    _load_balance.OfflineMachine(id);
                    _load_balance.ShowMachines();
                }
            }
        }
   };
} // namespace ns_control
