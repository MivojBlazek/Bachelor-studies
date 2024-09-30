/**
 * 
 * File: DnsMonitor.hpp
 * 
 * Author: Michal Blažek <xblaze38>
 * 
 */

#ifndef _DNS_MONITOR_
#define _DNS_MONITOR_

#include <string>

class DnsMonitor
{
public:
    int parseArguments(int argc, char *argv[]);
    void startCapturePackets();
    bool isVerbose() const;
    std::string getDomainsFile() const;
    std::string getTranslationsFile() const;

private:
    std::string interface;
    std::string pcapFile;
    bool verbose;
    std::string domainsFile;
    std::string translationsFile;
};

#endif //_DNS_MONITOR_
