#include "parser.hpp"
#include <iostream>
#include <arpa/inet.h>

void Parser::parse(const u_char* packet) {
    auto eth = reinterpret_cast<const EthHeader*>(packet);
    uint16_t eth_type = ntohs(eth->type);
    std::cout << "  [Eth] 0x" << std::hex << eth_type << std::dec << std::endl;

    if (eth_type == 0x0800) {
        auto ip = reinterpret_cast<const IpHeader*>(packet + sizeof(EthHeader));
        char src[INET_ADDRSTRLEN], dst[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &ip->src, src, sizeof(src));
        inet_ntop(AF_INET, &ip->dest, dst, sizeof(dst));
        std::cout << "  [IP] " << src << " -> " << dst
                  << "  Proto=" << int(ip->protocol) << std::endl;

        if (ip->protocol == 6) {
            size_t ihl = (ip->ver_ihl & 0x0F) * 4;
            auto tcp = reinterpret_cast<const TcpHeader*>(
                packet + sizeof(EthHeader) + ihl);
            std::cout << "  [TCP] " << ntohs(tcp->src_port)
                      << " -> " << ntohs(tcp->dest_port) << std::endl;
        }
        else if (ip->protocol == 17) {
            size_t ihl = (ip->ver_ihl & 0x0F) * 4;
            auto udp = reinterpret_cast<const UdpHeader*>(
                packet + sizeof(EthHeader) + ihl);
            std::cout << "  [UDP] " << ntohs(udp->src_port)
                      << " -> " << ntohs(udp->dest_port) << std::endl;
        }
    }

    std::cout << "----------------------------------------" << std::endl;
}

