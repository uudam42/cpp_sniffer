#pragma once

#include <pcap.h>
#include <string>

// 从 main.cpp 引入的全局中断标志
extern volatile bool stopFlag;

/**
 * @class Sniffer
 * @brief 封装 libpcap，提供抓包、过滤、统计功能
 */
class Sniffer {
public:
    Sniffer(const std::string& interface);
    ~Sniffer();

    /**
     * @brief 编译并安装 BPF 过滤器（如 "tcp port 80"）
     */
    bool setFilter(const std::string& expr);

    /**
     * @brief 开始手动抓包循环
     * @param count  要抓的包数 (0 = 无限)
     */
    void start(int count = 0);

    /**
     * @brief 返回已抓包总数
     */
    uint64_t getTotalCount() const;

private:
    pcap_t*  handle_;      ///< libpcap 会话句柄
    uint64_t packetCount_; ///< 已抓包计数

    // libpcap 回调：每抓一个包就调用
    static void packetHandler(u_char* args,
                              const pcap_pkthdr* header,
                              const u_char* packet);
};
