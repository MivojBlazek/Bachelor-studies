/**
 * 
 * File: PacketCapturer.cpp
 * 
 * Author: Michal Blažek <xblaze38>
 * 
 */

#include "PacketCapturer.hpp"

PacketCapturer::PacketCapturer(const std::string &_interface, const std::string &_pcapFile, DnsMonitor &_monitor)
    : interface(_interface),
    pcapFile(_pcapFile),
    monitor(_monitor)
{

}

void PacketCapturer::captureFromInterface()
{
    //TODO
}

void PacketCapturer::captureFromPcap()
{
    //TODO
}
