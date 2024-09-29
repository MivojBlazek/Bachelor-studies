/**
 * 
 * File: DnsHeader.hpp
 * 
 * Author: Michal Blažek <xblaze38>
 * 
 */

#ifndef _DNS_HEADER_
#define _DNS_HEADER_

#include <pcap.h>
#include <string>
#include <list>
#include <cstdint>
#include <ctime>

class DnsHeader
{
public:
    enum Parts {Questions, Answers, AuthorityRecords, AdditionalRecords};
    struct AdditionalHeaders
    {
        Parts part;
        std::string name;
        uint16_t type;
        uint16_t classType;
    };
    
    DnsHeader(const u_char *_data);
    ~DnsHeader();
    uint16_t getCountOf(Parts part) const;
    bool isQueryOrResponse() const;
    uint16_t getID() const;
    std::string getStringFlags() const;
    static std::string decodeDomainName(const u_char *data, int *offset);
    std::list<AdditionalHeaders *> getListOfHeaders();

private:
    uint16_t id;
    uint16_t flags;
    uint16_t questionCount;
    uint16_t answerCount;
    uint16_t authorityCount;
    uint16_t additionalCount;
    bool isQuery;
    std::list<AdditionalHeaders *> listOfAddRecords;
};

#endif //_DNS_HEADER_
