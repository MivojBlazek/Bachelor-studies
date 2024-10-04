/**
 * 
 * @file: DnsHeader.cpp
 * 
 * @author: Michal Blažek <xblaze38>
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

    // Load DNS header
    id = ntohs(dnsHeader[0]);
    flags = ntohs(dnsHeader[1]);
    questionCount = ntohs(dnsHeader[2]);
    answerCount = ntohs(dnsHeader[3]);
    authorityCount = ntohs(dnsHeader[4]);
    additionalCount = ntohs(dnsHeader[5]);
    isQuery = !(flags & 0x8000);

    // Load additional headers
    int offset = 12;
    int qCnt = questionCount;
    int aCnt = answerCount;
    int auCnt = authorityCount;
    int adCnt = additionalCount;
    listOfAddRecords.clear();

    while (qCnt != 0)
    {
        AdditionalHeaders *newHeader = new AdditionalHeaders();

        newHeader->part = Questions;
        qCnt--;
        
        newHeader->name = decodeDomainName(dnsData, &offset);
        if (!newHeader->name.empty())
        {
            domainNames.insert(newHeader->name);
            newHeader->name += ".";
        }
        newHeader->type = ntohs(*reinterpret_cast<const uint16_t *>(dnsData + offset));
        offset += 2;
        newHeader->classType = ntohs(*reinterpret_cast<const uint16_t *>(dnsData + offset));
        offset += 2;

        newHeader->TTL = 0;
        newHeader->rData = "";

        // Only supported types
        if (!(newHeader->type == A) &&
            !(newHeader->type == AAAA) &&
            !(newHeader->type == NS) &&
            !(newHeader->type == MX) &&
            !(newHeader->type == SOA) &&
            !(newHeader->type == CNAME) &&
            !(newHeader->type == SRV))
        {
            newHeader->type = NotSupported;
        }

        listOfAddRecords.push_back(newHeader);
    }

    while ((aCnt + auCnt + adCnt) != 0)
    {
        AdditionalHeaders *newHeader = new AdditionalHeaders();
        
        if (aCnt > 0)
        {
            newHeader->part = Answers;
            aCnt--;
        }
        else if (auCnt > 0)
        {
            newHeader->part = AuthorityRecords;
            auCnt--;
        }
        else if (adCnt > 0)
        {
            newHeader->part = AdditionalRecords;
            adCnt--;
        }
        
        newHeader->name = decodeDomainName(dnsData, &offset);
        if (!newHeader->name.empty())
        {
            domainNames.insert(newHeader->name);
            newHeader->name += ".";
        }
        newHeader->type = ntohs(*reinterpret_cast<const uint16_t *>(dnsData + offset));
        offset += 2;
        newHeader->classType = ntohs(*reinterpret_cast<const uint16_t *>(dnsData + offset));
        offset += 2;
        newHeader->TTL = ntohl(*reinterpret_cast<const uint32_t *>(dnsData + offset));
        offset += 4;

        uint16_t rDataLength = ntohs(*reinterpret_cast<const uint16_t *>(dnsData + offset));
        offset += 2;
        if (newHeader->type == A)
        {
            char ipv4[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, dnsData + offset, ipv4, INET_ADDRSTRLEN);
            newHeader->rData = std::string(ipv4);

            if (newHeader->part == Answers || newHeader->part == AdditionalRecords)
            {
                std::string translation = newHeader->name.substr(0, newHeader->name.size() - 1) + " " + newHeader->rData;
                translations.insert(translation);
            }
        }
        else if (newHeader->type == AAAA)
        {
            char ipv6[INET6_ADDRSTRLEN];
            inet_ntop(AF_INET6, dnsData + offset, ipv6, INET6_ADDRSTRLEN);
            newHeader->rData = std::string(ipv6);

            if (newHeader->part == Answers || newHeader->part == AdditionalRecords)
            {
                std::string translation = newHeader->name.substr(0, newHeader->name.size() - 1) + " " + newHeader->rData;
                translations.insert(translation);
            }
        }
        else if (newHeader->type == NS ||
                 newHeader->type == MX ||
                 newHeader->type == SOA ||
                 newHeader->type == CNAME ||
                 newHeader->type == SRV)
        {
            int tmpOffset = offset;
            newHeader->rData = decodeDomainName(dnsData, &tmpOffset);
            if (!newHeader->rData.empty())
            {
                domainNames.insert(newHeader->rData);
                newHeader->rData += ".";
            }
        }
        else
        {
            newHeader->type = NotSupported;
        }
        offset += rDataLength;

        listOfAddRecords.push_back(newHeader);
    }
}

DnsHeader::~DnsHeader()
{
    for (auto header : listOfAddRecords)
    {
        delete header;
    }
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

std::string DnsHeader::decodeDomainName(const u_char *data, int *offset)
{
    std::string name;
    bool jumped = false;

    while (data[*offset] != 0)
    {
        // Check if there is jump (byte 0xC0, first 2 bits)
        if ((data[*offset] & 0xC0) == 0xC0)
        {
            uint16_t pointer = ((data[*offset] & 0x3F) << 8) | data[*offset + 1]; // Last 14 bits
            *offset += 2;

            int point = pointer;
            std::string part = decodeDomainName(data, &point);
            if (!name.empty() && !part.empty())
            {
                name += ".";
            }
            name += part;
            jumped = true;
            break;
        }
        else
        {
            int len = data[*offset];
            (*offset)++;

            if (!name.empty()) {
                name += ".";
            }

            name += std::string(reinterpret_cast<const char *>(data + *offset), len);
            (*offset) += len;
        }
    }

    if (!jumped)
    {
        (*offset)++;
    }
    return name;
}

std::list<DnsHeader::AdditionalHeaders *> DnsHeader::getListOfHeaders()
{
    return listOfAddRecords;
}

std::set<std::string> DnsHeader::getDomains()
{
    return domainNames;
}

std::set<std::string> DnsHeader::getTranslations()
{
    return translations;
}
