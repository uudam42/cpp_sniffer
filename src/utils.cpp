#include "utils.hpp"
#include <iostream>
#include <chrono>
#include <ctime>
#include <iomanip> // for std::put_time

std::ofstream Log::ofs_;
std::mutex      Log::mtx_;

void Log::init(const std::string& path) {
    ofs_.open(path, std::ios::app);
    if (!ofs_) {
        std::cerr << "Failed to open log file: " << path << std::endl;
        std::exit(1);
    }
}

void Log::info(const std::string& msg) {
    std::lock_guard<std::mutex> locker(mtx_);
    // 写时间戳
    auto now = std::chrono::system_clock::to_time_t(
                   std::chrono::system_clock::now());
    ofs_ << std::put_time(std::localtime(&now), "%Y-%m-%d %H:%M:%S")
         << " " << msg << std::endl;
}
