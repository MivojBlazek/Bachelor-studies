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
        std::cout << "Timestamp: " << timeBuffer << std::endl
                  << "SrcIP: " << srcIP << std::endl
                  << "DstIP: " << dstIP << std::endl
                  << "SrcPort: UDP/" << sourcePort << std::endl
                  << "DstPort: UDP/" << destPort << std::endl
                  << "Identifier: 0x" << std::hex << std::uppercase << dnsHeader.getID() << std::dec << std::nouppercase << std::endl
                  << "Flags: " << dnsHeader.getStringFlags() << std::endl
                  << std::endl;

        //TODO 4 more headers
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
}

