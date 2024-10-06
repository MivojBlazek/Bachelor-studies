/**
 * 
 * @file: Packet.cpp
 * 
 * @author: Michal Blažek <xblaze38>
 * 
 */

#include "Packet.hpp"
#include "DnsHeader.hpp"
#include <iostream>
#include <ctime>
#include <net/ethernet.h> // netinet/ether.h is not on eva
#include <netinet/ip.h>
#include <netinet/udp.h> // there is no struct udphdr.dest and .source on eva (used uh_dport and uh_sport)
#include <arpa/inet.h>
#include <cstring>
#include <list>
#include <map>
#include <fstream>

Packet::Packet(const pcap_pkthdr *_header, const u_char *_data, int _length)
    : header(_data),
    headerLength(_length)
{
    data = _data + sizeof(struct ether_header);
    dataLength = _length - sizeof(struct ether_header);

    // IP addresses from IP header
    const struct ip *IPHeader = reinterpret_cast<const struct ip *>(data);
    char srcIPBuf[INET_ADDRSTRLEN];
    char dstIPBuf[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(IPHeader->ip_src), srcIPBuf, INET_ADDRSTRLEN);
    inet_ntop(AF_INET, &(IPHeader->ip_dst), dstIPBuf, INET_ADDRSTRLEN);
    srcIP = srcIPBuf;
    dstIP = dstIPBuf;

    // IP header size
    const struct ip *IPHeader2 = reinterpret_cast<const struct ip *>(data);
    size_t IPHeaderLength = IPHeader2->ip_hl * 4;

    // Ports from UDP header
    const u_char *UDPData = data + IPHeaderLength;
    const struct udphdr *UDPHeader = reinterpret_cast<const struct udphdr *>(UDPData);
    destPort = ntohs(UDPHeader->uh_dport);
    sourcePort = ntohs(UDPHeader->uh_sport);
    
    timestamp = _header->ts.tv_sec;
}

void Packet::printPacket(bool verbose, std::string domainsFile, std::string translationsFile) const
{
    DnsHeader dnsHeader(data);

    // Format timestamp to readable string
    std::tm *tm = std::localtime(&timestamp);
    char timeBuffer[20];
    std::strftime(timeBuffer, sizeof(timeBuffer), "%Y-%m-%d %H:%M:%S", tm);
 
    if (verbose)
    {
        // Main information in verbose mode
        std::cout << "Timestamp: " << timeBuffer << std::endl
                  << "SrcIP: " << srcIP << std::endl
                  << "DstIP: " << dstIP << std::endl
                  << "SrcPort: UDP/" << sourcePort << std::endl
                  << "DstPort: UDP/" << destPort << std::endl
                  << "Identifier: 0x" << std::hex << std::uppercase << dnsHeader.getID() << std::dec << std::nouppercase << std::endl
                  << "Flags: " << dnsHeader.getStringFlags() << std::endl;

        // Headers of additional sections
        bool first;
        std::map<uint16_t, std::string> dnsTypeToString = {
            {DnsHeader::A, "A"},
            {DnsHeader::AAAA, "AAAA"},
            {DnsHeader::NS, "NS"},
            {DnsHeader::MX, "MX"},
            {DnsHeader::SOA, "SOA"},
            {DnsHeader::CNAME, "CNAME"},
            {DnsHeader::SRV, "SRV"}
        };
        std::map<uint16_t, std::string> dnsClassToString = {
            {1, "IN"},
            {2, "CS"},
            {3, "CH"},
            {4, "HS"},
            {255, "*"}
        };
        std::list<DnsHeader::AdditionalHeaders *> listOfAddHeaders = dnsHeader.getListOfHeaders();
        for (int i = 0; i < dnsHeader.getCountOf(DnsHeader::Questions); i++)
        {
            if (i == 0)
            {
                first = true;
            }

            DnsHeader::AdditionalHeaders *header = listOfAddHeaders.front();
            listOfAddHeaders.pop_front();
            if (first)
            {
                if (header->type != DnsHeader::NotSupported)
                {
                    first = false;
                    std::cout << std::endl;
                    std::cout << "[Question Section]" << std::endl;
                }
            }

            if (header->type != DnsHeader::NotSupported)
            {
                std::string classString, typeString;
                auto it = dnsClassToString.find(header->classType);
                if (it != dnsClassToString.end())
                {
                    classString = it->second;
                }
                auto it2 = dnsTypeToString.find(header->type);
                if (it2 != dnsTypeToString.end())
                {
                    typeString = it2->second;
                }

                std::cout << header->name << " " << classString << " " << typeString << std::endl;
            }
        }
        for (int i = 0; i < dnsHeader.getCountOf(DnsHeader::Answers); i++)
        {
            if (i == 0)
            {
                first = true;
            }

            DnsHeader::AdditionalHeaders *header = listOfAddHeaders.front();
            listOfAddHeaders.pop_front();
            if (first)
            {
                if (header->type != DnsHeader::NotSupported)
                {
                    first = false;
                    std::cout << std::endl;
                    std::cout << "[Answer Section]" << std::endl;
                }
            }

            if (header->type != DnsHeader::NotSupported)
            {
                std::string classString, typeString;
                auto it = dnsClassToString.find(header->classType);
                if (it != dnsClassToString.end())
                {
                    classString = it->second;
                }
                auto it2 = dnsTypeToString.find(header->type);
                if (it2 != dnsTypeToString.end())
                {
                    typeString = it2->second;
                }

                std::cout << header->name << " " << header->TTL << " " << classString << " " << typeString << " " << header->rData << std::endl;
            }
        }
        for (int i = 0; i < dnsHeader.getCountOf(DnsHeader::AuthorityRecords); i++)
        {
            if (i == 0)
            {
                first = true;
            }

            DnsHeader::AdditionalHeaders *header = listOfAddHeaders.front();
            listOfAddHeaders.pop_front();
            if (first)
            {
                if (header->type != DnsHeader::NotSupported)
                {
                    first = false;
                    std::cout << std::endl;
                    std::cout << "[Authority Section]" << std::endl;
                }
            }

            if (header->type != DnsHeader::NotSupported)
            {
                std::string classString, typeString;
                auto it = dnsClassToString.find(header->classType);
                if (it != dnsClassToString.end())
                {
                    classString = it->second;
                }
                auto it2 = dnsTypeToString.find(header->type);
                if (it2 != dnsTypeToString.end())
                {
                    typeString = it2->second;
                }

                std::cout << header->name << " " << header->TTL << " " << classString << " " << typeString << " " << header->rData << std::endl;
            }
        }
        for (int i = 0; i < dnsHeader.getCountOf(DnsHeader::AdditionalRecords); i++)
        {
            if (i == 0)
            {
                first = true;
            }

            DnsHeader::AdditionalHeaders *header = listOfAddHeaders.front();
            listOfAddHeaders.pop_front();
            if (first)
            {
                if (header->type != DnsHeader::NotSupported)
                {
                    first = false;
                    std::cout << std::endl;
                    std::cout << "[Additional Section]" << std::endl;
                }
            }

            if (header->type != DnsHeader::NotSupported)
            {
                std::string classString, typeString;
                auto it = dnsClassToString.find(header->classType);
                if (it != dnsClassToString.end())
                {
                    classString = it->second;
                }
                auto it2 = dnsTypeToString.find(header->type);
                if (it2 != dnsTypeToString.end())
                {
                    typeString = it2->second;
                }

                std::cout << header->name << " " << header->TTL << " " << classString << " " << typeString << " " << header->rData << std::endl;
            }
        }

        // End of verbose record
        std::cout << "====================" << std::endl << std::endl;
    }
    else
    {
        // Print the simplified output
        std::cout << timeBuffer << " " << srcIP << " -> " << dstIP << " (";

        std::cout << (dnsHeader.isQueryOrResponse() ? "Q" : "R") << " "
                  << dnsHeader.getCountOf(DnsHeader::Questions) << "/" 
                  << dnsHeader.getCountOf(DnsHeader::Answers) << "/" 
                  << dnsHeader.getCountOf(DnsHeader::AuthorityRecords) << "/" 
                  << dnsHeader.getCountOf(DnsHeader::AdditionalRecords) << ")" << std::endl;
    }

    // Domains and translations processing
    if (!domainsFile.empty() || domainsFile != "")
    {
        fillDomainsFile(dnsHeader.getDomains(), domainsFile);
    }
    if (!translationsFile.empty() || translationsFile != "")
    {
        fillTranslationsFile(dnsHeader.getTranslations(), translationsFile);
    }
}

void Packet::fillDomainsFile(std::set<std::string> domainNames, std::string domainsFile) const
{
    std::set<std::string> allDomains;

    // Save file to allDomains set to have unique records
    std::ifstream inputFile(domainsFile);
    if (inputFile.is_open())
    {
        std::string line;
        while (std::getline(inputFile, line))
        {
            allDomains.insert(line);
        }
        inputFile.close();
    }

    // Add new records
    for (const std::string &domain : domainNames)
    {
        allDomains.insert(domain);
    }

    // Print all records to file
    std::ofstream outputFile(domainsFile);
    if (outputFile.is_open())
    {
        for (const std::string &domain : allDomains)
        {
            outputFile << domain << std::endl;
        }
        outputFile.close();
    }
}

void Packet::fillTranslationsFile(std::set<std::string> translations, std::string translationsFile) const
{
    std::set<std::string> allTranslations;

    // Save file to allTranslations set to have unique records
    std::ifstream inputFile(translationsFile);
    if (inputFile.is_open())
    {
        std::string line;
        while (std::getline(inputFile, line))
        {
            allTranslations.insert(line);
        }
        inputFile.close();
    }

    // Add new records
    for (const std::string &translation : translations)
    {
        allTranslations.insert(translation);
    }

    // Print all records to file
    std::ofstream outputFile(translationsFile);
    if (outputFile.is_open())
    {
        for (const std::string &translation : allTranslations)
        {
            outputFile << translation << std::endl;
        }
        outputFile.close();
    }
}
