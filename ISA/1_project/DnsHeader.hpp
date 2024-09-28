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
#include <cstdint>
#include <ctime>

class DnsHeader
{
public:
    enum Parts {Questions, Answers, AuthorityRecords, AdditionalRecords};
    DnsHeader(const u_char *_data);
    uint16_t getCountOf(Parts part) const;
    bool isQueryOrResponse() const;
    uint16_t getID() const;
    std::string getStringFlags() const;

private:
    uint16_t id; // identification number
    uint16_t flags; // flags
    uint16_t questionCount; // number of questions
    uint16_t answerCount; // number of answers
    uint16_t authorityCount; // number of authority records
    uint16_t additionalCount;
    bool isQuery; // true for query, false for response
};

#endif //_DNS_HEADER_
