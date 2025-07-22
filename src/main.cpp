#include "sniffer.hpp"
#include <iostream>
#include <csignal>
#include <unistd.h>
#include "utils.hpp"

// 全局中断标志
volatile bool stopFlag = false;
void handleSignal(int) { stopFlag = true; }

void printUsage(const char* prog) {
    std::cout
      << "Usage: " << prog << " [options]\n"
      << "  -i <interface>   Network interface (default: en0)\n"
      << "  -f <filter>      BPF filter (e.g. \"tcp port 80\")\n"
      << "  -n <count>       Number of packets (0=infinite)\n"
      << "  -o <logfile>     Write log to specified file\n"
      << "  -h               Show help\n";
}

int main(int argc, char* argv[]) {
    std::string iface = "en0", filterExpr = "";
    std::string logPath;
    int packetCount = 0;
    int opt;
    while ((opt = getopt(argc, argv, "i:f:n:o:h")) != -1) {
        switch (opt) {
            case 'i': iface       = optarg; break;
            case 'f': filterExpr  = optarg; break;
            case 'n': packetCount = std::stoi(optarg); break;
            case 'o': logPath = optarg; break;
            case 'h':
            default:
                printUsage(argv[0]);
                return (opt == 'h') ? 0 : 1;
        }
    }

    if (!logPath.empty()) {
        Log::init(logPath);
        Log::info("=== Start sniffing ===");
    }


    std::signal(SIGINT, handleSignal);

    std::cout << "Interface: " << iface
              << "  Filter: " << (filterExpr.empty()?"none":filterExpr)
              << "  Count: " << packetCount << std::endl;

    Sniffer sniffer(iface);
    if (!filterExpr.empty() && !sniffer.setFilter(filterExpr)) {
        std::cerr << "Error setting filter: " << filterExpr << std::endl;
        return 1;
    }

    sniffer.start(packetCount);

    std::string result = std::string("*** Capture ") +
                         (stopFlag?"interrupted":"complete") +
                         ", total packets: " +
                         std::to_string(sniffer.getTotalCount()) + " ***";
    if (!logPath.empty()) Log::info(result);


    sniffer.start(packetCount);
    return 0;
}
