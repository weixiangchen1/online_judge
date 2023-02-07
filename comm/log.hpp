#pragma once

#include <iostream>
#include <string>

#include "../comm/util.hpp"

namespace ns_log
{
    using namespace ns_util;
    // 日志等级
    enum
    {
        INFO,
        DEBUG,
        WARING,
        ERROR,
        FATAL
    };

    inline std::ostream& Log(const std::string& level, const std::string& file_name, int line)
    {
        // 添加日志等级
        std::string message = "[";
        message += level;
        message += "]";

        // 添加文件名信息
        message += "[";
        message += file_name;
        message += "]";

        // 添加报错行号
        message += "[";
        message += std::to_string(line);
        message += "]";

        // 添加日志时间戳
        message += "[";
        message += TimeUtil::GetTimeStamp();
        message += "]";

        // cout 内部包含缓冲区，不刷新，数据会暂存在缓冲区
        std::cout << message;
        return std::cout;
    }

    // LOG(level) << "开放信息"  -- 开放性日志
    #define LOG(level) Log(#level, __FILE__, __LINE__)

}