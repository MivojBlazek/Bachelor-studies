/**
 * 
 * File: PacketCapturer.hpp
 * 
 * Author: Michal Blažek <xblaze38>
 * 
 */

#ifndef _PACKET_CAPTURER_
#define _PACKET_CAPTURER_

#include "DnsMonitor.hpp"
#include <pcap.h>
#include <string>

class PacketCapturer
{
public:
    PacketCapturer(const std::string &_interface, const std::string &_pcapFile, DnsMonitor &_monitor);
    void captureFromInterface();
    void captureFromPcap();

private:
    static void processPacket(u_char *args, const struct pcap_pkthdr *header, const u_char *packet);
    std::string interface;
    std::string pcapFile;
    DnsMonitor &monitor;
};

#endif //_PACKET_CAPTURER_
