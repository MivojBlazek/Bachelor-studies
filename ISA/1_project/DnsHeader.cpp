/**
 * 
 * File: DnsHeader.cpp
 * 
 * Author: Michal Blažek <xblaze38>
 * 
 */

#include "DnsHeader.hpp"
#include <iostream>
#include <cstring>
#include <arpa/inet.h>
#include <netinet/ip.h>
#include <netinet/udp.h>

DnsHeader::DnsHeader(const u_char *_data)
{
    // Skip IP header
    // Skip UDP header

    const struct ip *IPHeader = reinterpret_cast<const struct ip *>(_data);
    size_t IPHeaderLength = IPHeader->ip_hl * 4;

    const u_char *dnsData = _data + IPHeaderLength + sizeof(struct udphdr);
    const uint16_t *dnsHeader = reinterpret_cast<const uint16_t *>(dnsData);

    id = ntohs(dnsHeader[0]);
    flags = ntohs(dnsHeader[1]);
    questionCount = ntohs(dnsHeader[2]);
    answerCount = ntohs(dnsHeader[3]);
    authorityCount = ntohs(dnsHeader[4]);
    additionalCount = ntohs(dnsHeader[5]);
    isQuery = !(flags & 0x8000);
}

uint16_t DnsHeader::getCountOf(Parts part) const
{
    uint16_t count = 0;
    switch (part)
    {
    case Questions:
        count = questionCount;
        break;
    case Answers:
        count = answerCount;
        break;
    case AuthorityRecords:
        count = authorityCount;
        break;
    case AdditionalRecords:
        count = additionalCount;
        break;
    }
    return count;
}

bool DnsHeader::isQueryOrResponse() const
{
    return isQuery;
}

uint16_t DnsHeader::getID() const
{
    return id;
}

std::string DnsHeader::getStringFlags() const
{
    std::string flagsString;
    flagsString.clear();
    
    flagsString += "QR=" + std::to_string((flags & 0x8000) >> 15) + ", ";
    flagsString += "OPCODE=" + std::to_string((flags & 0x7800) >> 11) + ", ";
    flagsString += "AA=" + std::to_string((flags & 0x0400) >> 10) + ", ";
    flagsString += "TC=" + std::to_string((flags & 0x0200) >> 9) + ", ";
    flagsString += "RD=" + std::to_string((flags & 0x0100) >> 8) + ", ";
    flagsString += "RA=" + std::to_string((flags & 0x0080) >> 7) + ", ";
    flagsString += "AD=" + std::to_string((flags & 0x0020) >> 5) + ", ";
    flagsString += "CD=" + std::to_string((flags & 0x0010) >> 4) + ", ";
    flagsString += "RCODE=" + std::to_string(flags & 0x000F);

    return flagsString;
}
