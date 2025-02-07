/**
 * 
 * @file: main.cpp
 * 
 * @author: Michal Blažek <xblaze38>
 * 
 */

#include "DnsMonitor.hpp"

int main(int argc, char *argv[])
{
    DnsMonitor monitor;
    if (monitor.parseArguments(argc, argv))
    {
        return 1;
    }
    monitor.startCapturePackets();

    return 0;
}
