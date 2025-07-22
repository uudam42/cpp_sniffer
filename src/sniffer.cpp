#include "sniffer.hpp"
#include "parser.hpp"
#include <iostream>
#include <cstdlib>
#include <pcap.h>

// 构造：打开网卡接口
Sniffer::Sniffer(const std::string& interface)
  : handle_(nullptr), packetCount_(0)
{
    char errbuf[PCAP_ERRBUF_SIZE];
    handle_ = pcap_open_live(interface.c_str(), BUFSIZ, 1, 1000, errbuf);
    if (!handle_) {
        std::cerr << "Error opening interface " << interface
                  << ": " << errbuf << std::endl;
        std::exit(EXIT_FAILURE);
    }
    std::cout << "Opened interface: " << interface << std::endl;
}

// 析构：关闭会话
Sniffer::~Sniffer() {
    if (handle_) pcap_close(handle_);
}

// 安装 BPF 过滤器
bool Sniffer::setFilter(const std::string& expr) {
    struct bpf_program fp;
    if (pcap_compile(handle_, &fp, expr.c_str(), 1, PCAP_NETMASK_UNKNOWN) < 0) {
        std::cerr << "pcap_compile failed: " << pcap_geterr(handle_) << std::endl;
        return false;
    }
    if (pcap_setfilter(handle_, &fp) < 0) {
        std::cerr << "pcap_setfilter failed: " << pcap_geterr(handle_) << std::endl;
        return false;
    }
    std::cout << "Filter installed: " << expr << std::endl;
    return true;
}

// 手动循环抓包
void Sniffer::start(int count) {
    packetCount_ = 0;
    std::cout << "Capturing " << (count ? std::to_string(count) : "unlimited")
              << " packets... (press Ctrl-C to stop early)" << std::endl;

    // 循环调用 pcap_dispatch，每次处理 1 个包或等待超时
    while (!stopFlag && (count <= 0 || packetCount_ < (uint64_t)count)) {
        int ret = pcap_dispatch(handle_, 1, packetHandler,
                                reinterpret_cast<u_char*>(this));
        if (ret < 0) {
            std::cerr << "pcap_dispatch error: " << pcap_geterr(handle_) << std::endl;
            break;
        }
        // ret == 0 表示超时无包，但我们依然检查 stopFlag
    }

    std::cout << "\nCapture "
              << (stopFlag ? "interrupted" : "complete")
              << ", total packets: " << packetCount_ << std::endl;
}

// 回调：统计并调用解析
void Sniffer::packetHandler(u_char* args,
                            const pcap_pkthdr* header,
                            const u_char* packet)
{
    Sniffer* self = reinterpret_cast<Sniffer*>(args);
    self->packetCount_++;
    std::cout << "[#" << self->packetCount_ << "] Len="
              << header->len << " bytes" << std::endl;
    Parser::parse(packet);
}

uint64_t Sniffer::getTotalCount() const {
    return packetCount_;
}
