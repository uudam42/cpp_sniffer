#pragma once
#include <cstdint>
#include <netinet/in.h>

struct EthHeader { uint8_t dest[6], src[6]; uint16_t type; };
struct IpHeader {
    uint8_t ver_ihl, tos; uint16_t tot_len, id, frag_off;
    uint8_t ttl, protocol; uint16_t check; in_addr src, dest;
};
struct TcpHeader { uint16_t src_port, dest_port; uint32_t seq, ack_seq;
    uint8_t data_offset, flags; uint16_t window, check, urg_ptr;
};
struct UdpHeader { uint16_t src_port, dest_port, len, check; };

class Parser {
public:
    static void parse(const u_char* packet);
};
