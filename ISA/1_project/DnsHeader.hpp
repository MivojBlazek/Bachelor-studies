/**
 * 
 * @file: DnsHeader.hpp
 * 
 * @author: Michal Blažek <xblaze38>
 * 
 */

#ifndef _DNS_HEADER_
#define _DNS_HEADER_

#include <pcap.h>
#include <string>
#include <list>
#include <set>
#include <cstdint>
#include <ctime>

class DnsHeader
{
public:
    enum SupportedTypes
    {
        NotSupported = 65000,
        A = 1,
        AAAA = 28,
        NS = 2,
        MX = 15,
        SOA = 6,
        CNAME = 5,
        SRV = 33
    };
    enum Parts {Questions, Answers, AuthorityRecords, AdditionalRecords};
    struct AdditionalHeaders
    {
        Parts part;
        std::string name;
        uint16_t type;
        uint16_t classType;
        uint32_t TTL;
        std::string rData;
    };
    
    DnsHeader(const u_char *_data);
    ~DnsHeader();
    uint16_t getCountOf(Parts part) const;
    bool isQueryOrResponse() const;
    uint16_t getID() const;
    std::string getStringFlags() const;
    static std::string decodeDomainName(const u_char *data, int *offset);
    std::list<AdditionalHeaders *> getListOfHeaders();
    std::set<std::string> getDomains();
    std::set<std::string> getTranslations();

private:
    uint16_t id;
    uint16_t flags;
    uint16_t questionCount;
    uint16_t answerCount;
    uint16_t authorityCount;
    uint16_t additionalCount;
    bool isQuery;
    std::list<AdditionalHeaders *> listOfAddRecords;
    std::set<std::string> domainNames;
    std::set<std::string> translations;
};

#endif //_DNS_HEADER_
