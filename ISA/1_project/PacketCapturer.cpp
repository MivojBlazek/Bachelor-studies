/**
 * 
 * @file: PacketCapturer.cpp
 * 
 * @author: Michal Blažek <xblaze38>
 * 
 */

#include "PacketCapturer.hpp"
#include "Packet.hpp"
#include <iostream>
#include <pcap.h>
#include <netinet/in.h>
#include <netinet/ether.h>
#include <netinet/ip.h>
#include <netinet/ip6.h>
#include <netinet/udp.h>
#include <net/ethernet.h>
#include <arpa/inet.h>
#include <time.h>
#include <netdb.h>

PacketCapturer::PacketCapturer(const std::string &_interface, const std::string &_pcapFile, DnsMonitor &_monitor)
    : interface(_interface),
    pcapFile(_pcapFile),
    monitor(_monitor)
{

}

void PacketCapturer::captureFromInterface()
{
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *handle;

    // Open device for capture
    handle = pcap_open_live(interface.c_str(), BUFSIZ, 1, 1000, errbuf);
    if (handle == nullptr)
    {
        std::cerr << "Error: Couldn't open device " << interface << ": " << errbuf << std::endl;
        return;
    }

    struct bpf_program fp;
    std::string filter_exp = "udp port 53"; // Filter udp and dns
    bpf_u_int32 net = 0;

    if (pcap_compile(handle, &fp, filter_exp.c_str(), 0, net) == -1)
    {
        std::cerr << "Error: Couldn't parse filter " << filter_exp << ": " << pcap_geterr(handle) << std::endl;
        pcap_close(handle);
        return;
    }

    if (pcap_setfilter(handle, &fp) == -1)
    {
        std::cerr << "Error: Couldn't install filter " << filter_exp << ": " << pcap_geterr(handle) << std::endl;
        pcap_freecode(&fp);
        pcap_close(handle);
        return;
    }

    // Capturing
    pcap_loop(handle, 0, processPacket, reinterpret_cast<u_char *>(&monitor));

    // Clean up
    pcap_freecode(&fp);
    pcap_close(handle);
}

void PacketCapturer::captureFromPcap()
{
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *handle;

    // Open device for capture from file
    handle = pcap_open_offline(pcapFile.c_str(), errbuf);
    if (handle == nullptr)
    {
        std::cerr << "Error: Couldn't open device " << pcapFile << ": " << errbuf << std::endl;
        return;
    }

    struct bpf_program fp;
    std::string filter_exp = "udp port 53"; // Filter udp and dns
    bpf_u_int32 net = 0;

    if (pcap_compile(handle, &fp, filter_exp.c_str(), 0, net) == -1)
    {
        std::cerr << "Error: Couldn't parse filter " << filter_exp << ": " << pcap_geterr(handle) << std::endl;
        pcap_close(handle);
        return;
    }

    if (pcap_setfilter(handle, &fp) == -1)
    {
        std::cerr << "Error: Couldn't install filter " << filter_exp << ": " << pcap_geterr(handle) << std::endl;
        pcap_freecode(&fp);
        pcap_close(handle);
        return;
    }

    // Capturing
    std::cout << "Listening for DNS traffic on " << interface << "..." << std::endl;
    pcap_loop(handle, 0, processPacket, reinterpret_cast<u_char *>(&monitor));

    // Clean up
    pcap_freecode(&fp);
    pcap_close(handle);
}

void PacketCapturer::processPacket(u_char *args, const struct pcap_pkthdr *header, const u_char *packet)
{
    DnsMonitor *dnsMonitor = reinterpret_cast<DnsMonitor *>(args);

    Packet pkt(header, packet, header->len);
    pkt.printPacket(dnsMonitor->isVerbose(), dnsMonitor->getDomainsFile(), dnsMonitor->getTranslationsFile());
}
