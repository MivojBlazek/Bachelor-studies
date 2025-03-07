/**
 * 
 * @file: DnsMonitor.cpp
 * 
 * @author: Michal Blažek <xblaze38>
 * 
 */

#include "DnsMonitor.hpp"
#include "PacketCapturer.hpp"
#include <iostream>
#include <unistd.h>
#include <filesystem>
#include <csignal>

// Allocate storage for static property
PacketCapturer *DnsMonitor::capturer = nullptr;

void DnsMonitor::exitProgram(int signal)
{
    // Clean up memory
    delete capturer;
    capturer = nullptr;
    std::exit(signal);
}

int DnsMonitor::parseArguments(int argc, char *argv[])
{
    interface.clear();
    pcapFile.clear();
    verbose = false;
    domainsFile.clear();
    translationsFile.clear();
    
    // Process program arguments
    int opt;
    while ((opt = getopt(argc, argv, "i:p:vd:t:")) != -1)
    {
        switch (opt)
        {
            case 'i':
                interface = optarg;
                break;
            case 'p':
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
                std::cout << "Usage:" << std::endl << "./dns-monitor (-i <interface> | -r <pcapfile>) [-v] [-d <domainsfile>] [-t <translationsfile>]" << std::endl;
                return 1;
        }
    }

    // Check if -i or -r was specified but not both
    if ((interface.empty() && pcapFile.empty()) || (!interface.empty() && !pcapFile.empty()))
    {
        std::cerr << "You must specify -i <interface> or -p <pcapfile> but not both." << std::endl;
        return 1;
    }

    // Check if file for domains exists and remove it
    if (!domainsFile.empty())
    {
        if (std::filesystem::exists(domainsFile))
        {
            std::filesystem::remove(domainsFile);
        }
    }

    // Check if file for translations exists and remove it
    if (!translationsFile.empty())
    {
        if (std::filesystem::exists(translationsFile))
        {
            std::filesystem::remove(translationsFile);
        }
    }
    return 0;
}

void DnsMonitor::startCapturePackets()
{
    capturer = new PacketCapturer(interface, pcapFile, *this);
    std::signal(SIGINT, DnsMonitor::exitProgram);
    std::signal(SIGTERM, DnsMonitor::exitProgram);
    std::signal(SIGQUIT, DnsMonitor::exitProgram);

    if (!interface.empty())
    {
        capturer->captureFromInterface();
    }
    else if (!pcapFile.empty())
    {
        capturer->captureFromPcap();
    }
    delete capturer;
    capturer = nullptr;
}

bool DnsMonitor::isVerbose() const
{
    return verbose;
}

std::string DnsMonitor::getDomainsFile() const
{
    if (!domainsFile.empty())
    {
        return domainsFile;
    }
    return "";
}

std::string DnsMonitor::getTranslationsFile() const
{
    if (!translationsFile.empty())
    {
        return translationsFile;
    }
    return "";
}
