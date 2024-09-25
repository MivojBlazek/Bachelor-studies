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
#include <string>

class PacketCapturer
{
public:
    PacketCapturer(const std::string &interface, const std::string &pcapFile, DnsMonitor &monitor);
    void captureFromInterface();
    void captureFromPcap();

private:
    std::string interface;
    std::string pcapFile;
    DnsMonitor &monitor;
};

#endif //_PACKET_CAPTURER_
