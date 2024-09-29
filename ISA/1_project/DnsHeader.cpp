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
        newHeader->type = ntohs(*reinterpret_cast<const uint16_t *>(dnsData + offset));
        offset += 2;
        newHeader->classType = ntohs(*reinterpret_cast<const uint16_t *>(dnsData + offset));
        offset += 2;

        listOfAddRecords.push_back(newHeader);

        std::cout << "Parsed: " << newHeader->name << ", Type: " << newHeader->type
                  << ", Class: " << newHeader->classType << std::endl; //! DEBUG
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
        
        if ((dnsData[offset] & 0xC0) == 0xC0)
        {
            // Compressed version of name
            uint16_t pointer = ((dnsData[offset] & 0x3F) << 8) | dnsData[offset + 1]; // Last 14 bits
            int tmpOffset = pointer;
            newHeader->name = "hello";//decodeDomainName(dnsData, &tmpOffset); //! DEBUG
            offset += 2;
        }
        else 
        {
            // Normal version of name
            newHeader->name = decodeDomainName(dnsData, &offset);
        }
        newHeader->type = ntohs(*reinterpret_cast<const uint16_t *>(dnsData + offset));
        offset += 2;
        newHeader->classType = ntohs(*reinterpret_cast<const uint16_t *>(dnsData + offset));
        offset += 2;
        //TODO offset until next header

        listOfAddRecords.push_back(newHeader);

        std::cout << "Parsed: " << newHeader->name << ", Type: " << newHeader->type
                  << ", Class: " << newHeader->classType << std::endl; //! DEBUG
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
    while (data[*offset] != 0) {
        int len = data[*offset];
        (*offset)++;
        if (!name.empty())
        {
            name += ".";
        }
        name += std::string(reinterpret_cast<const char *>(data + *offset), len);
        (*offset) = (*offset) + len;
    }
    (*offset)++;
    return name;
}

std::list<DnsHeader::AdditionalHeaders *> DnsHeader::getListOfHeaders()
{
    return listOfAddRecords;
}
