/**
 * 
 * File: Packet.hpp
 * 
 * Author: Michal Blažek <xblaze38>
 * 
 */

#ifndef _PACKET_
#define _PACKET_

#include <pcap.h>
#include <string>
#include <cstdint>
#include <ctime>

class Packet
{
public:
    Packet(const struct pcap_pkthdr *_header, const u_char *_data, int _length);
    void printPacket(bool verbose) const;

private:
    const u_char *header;
    const u_char *data;
    int headerLength;
    int dataLength;
    std::string srcIP;
    std::string dstIP;
    uint16_t destPort;
    uint16_t sourcePort;
    std::time_t timestamp;
};

#endif //_PACKET_
