/**
 * 
 * File: DnsMonitor.cpp
 * 
 * Author: Michal Blažek <xblaze38>
 * 
 */

#include "DnsMonitor.hpp"
#include "PacketCapturer.hpp"
#include <iostream>
#include <unistd.h>

int DnsMonitor::parseArguments(int argc, char *argv[])
{
    interface.clear();
    pcapFile.clear();
    verbose = false;
    domainsFile.clear();
    translationsFile.clear();
    
    // Process program arguments
    int opt;
    while ((opt = getopt(argc, argv, "i:r:vd:t:")) != -1)
    {
        switch (opt)
        {
        case 'i':
            interface = optarg;
            break;
        case 'r':
            pcapFile = optarg;
            break;
        case 'v':
            verbose = true;
            break;
        case 'd':
            domainsFile = optarg;
            break;
        case 't':
            translationsFile = optarg;
            break;
        default:
            std::cout << "Usage:\n./dns-monitor (-i <interface> | -r <pcapfile>) [-v] [-d <domainsfile>] [-t <translationsfile>]\n";
            return 1;
        }
    }

    // Check if -i or -r was specified but not both
    if ((interface.empty() && pcapFile.empty()) || (!interface.empty() && !pcapFile.empty()))
    {
        std::cerr << "You must specify -i <interface> or -r <pcapfile> but not both.\n";
        return 1;
    }
    return 0;
}

void DnsMonitor::startCapturePackets()
{
    PacketCapturer capturer(interface, pcapFile, *this);

    if (!interface.empty())
    {
        capturer.captureFromInterface();
    }
    else if (!pcapFile.empty())
    {
        capturer.captureFromPcap();
    }
}

bool DnsMonitor::isVerbose() const
{
    return verbose;
}
