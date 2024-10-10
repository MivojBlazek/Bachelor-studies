/**
 * 
 * @file: PacketCapturer.hpp
 * 
 * @author: Michal Blažek <xblaze38>
 * 
 */

#ifndef _PACKET_CAPTURER_
#define _PACKET_CAPTURER_

#include "DnsMonitor.hpp"
#include <pcap.h>
#include <string>

// Forward declaration
class DnsMonitor;

class PacketCapturer
{
public:
    PacketCapturer(const std::string &_interface, const std::string &_pcapFile, DnsMonitor &_monitor);
    ~PacketCapturer();
    void captureFromInterface();
    void captureFromPcap();

private:
    static void processPacket(u_char *args, const struct pcap_pkthdr *header, const u_char *packet);
    std::string interface;
    std::string pcapFile;
    DnsMonitor &monitor;
    pcap_t *handle;
    struct bpf_program fp;
};

#endif //_PACKET_CAPTURER_
