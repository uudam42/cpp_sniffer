#pragma once
#include <string>
#include <fstream>
#include <mutex>

class Log {
public:
    // 打开日志文件（append 模式）
    static void init(const std::string& path);

    // 写一行日志（线程安全）
    static void info(const std::string& msg);

private:
    static std::ofstream ofs_;
    static std::mutex      mtx_;
};
