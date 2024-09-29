/**
 * 
 * File: Packet.cpp
 * 
 * Author: Michal Blažek <xblaze38>
 * 
 */

#include "Packet.hpp"
#include <iostream>
#include <iomanip>      // for std::put_time
#include <ctime>       // for std::time_t
#include <netinet/ip.h>  // for IP header
#include <netinet/udp.h>
#include <netinet/ether.h>
#include <arpa/inet.h>   // for inet_ntop
#include <cstring>
#include <list>
#include "DnsHeader.hpp"

Packet::Packet(const pcap_pkthdr *_header, const u_char *_data, int _length)
    : header(_data),
    headerLength(_length)
{
    data = _data + sizeof(struct ether_header);
    dataLength = _length - sizeof(struct ether_header);

    struct ip *IPHeader = (struct ip *)(_header + sizeof(struct ether_header));
    char srcIPBuf[INET_ADDRSTRLEN];
    char dstIPBuf[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(IPHeader->ip_src), srcIPBuf, INET_ADDRSTRLEN);
    inet_ntop(AF_INET, &(IPHeader->ip_dst), dstIPBuf, INET_ADDRSTRLEN);
    srcIP = srcIPBuf;
    dstIP = dstIPBuf;


    // IP header size
    const struct ip *IPHeader2 = reinterpret_cast<const struct ip *>(data);
    size_t IPHeaderLength = IPHeader2->ip_hl * 4;

    const u_char *UDPData = data + IPHeaderLength;
    const struct udphdr *UDPHeader = reinterpret_cast<const struct udphdr *>(UDPData);
    destPort = ntohs(UDPHeader->dest);
    sourcePort = ntohs(UDPHeader->source);
    
    timestamp = _header->ts.tv_sec;
}

void Packet::printPacket(bool verbose) const
{
    DnsHeader dnsHeader(data);

    // Format the timestamp into a readable string
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
        std::list<DnsHeader::AdditionalHeaders *> listOfAddHeaders = dnsHeader.getListOfHeaders();
        for (int i = 0; i < dnsHeader.getCountOf(DnsHeader::Questions); i++)
        {
            if (i == 0)
            {
                std::cout << std::endl;
                std::cout << "[Question Section]" << std::endl;
            }
            DnsHeader::AdditionalHeaders *header = listOfAddHeaders.front();
            listOfAddHeaders.pop_front();
            // std::string classString, typeString; //TODO
            // if (header->classType == 1)
            // {
            //     classString = "IN";
            // }
            // if (header->type == 1)
            // {
            //     typeString = "A";
            // }

            std::cout << header->name << ". " << header->classType << " " << header->type << std::endl;
        }
        for (int i = 0; i < dnsHeader.getCountOf(DnsHeader::Answers); i++)
        {
            if (i == 0)
            {
                std::cout << std::endl;
                std::cout << "[Answer Section]" << std::endl;
            }
            DnsHeader::AdditionalHeaders *header = listOfAddHeaders.front();
            listOfAddHeaders.pop_front();
            //TODO
        }
        for (int i = 0; i < dnsHeader.getCountOf(DnsHeader::AuthorityRecords); i++)
        {
            if (i == 0)
            {
                std::cout << std::endl;
                std::cout << "[Authority Section]" << std::endl;
            }
            DnsHeader::AdditionalHeaders *header = listOfAddHeaders.front();
            listOfAddHeaders.pop_front();
            //TODO
        }
        for (int i = 0; i < dnsHeader.getCountOf(DnsHeader::AdditionalRecords); i++)
        {
            if (i == 0)
            {
                std::cout << std::endl;
                std::cout << "[Additional Section]" << std::endl;
            }
            DnsHeader::AdditionalHeaders *header = listOfAddHeaders.front();
            listOfAddHeaders.pop_front();
            //TODO
        }

        // End of verbose record
        std::cout << std::endl << "====================" << std::endl << std::endl;
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
}

