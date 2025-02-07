# Project 2 - DELTA: L2/L3 Scanner

## Assignment
1. Create a simple network ICMP(v6), ARP/NDP scanner in C/C++/C#. The program discovers what devices are available from a selected range of IP addresses. It prints to standard output the availability status of the given IP addresses at the L2 and L3 layers (14 pts.) 
2. Create relevant manual/documentation for the project (6 pts.)

## Specification
The application scans for presence of L2 and L3 devices on given network segment(s). 

Packets/Frames should be sent using raw sockets. If needed, you can eavesdrop on the responses using the libpcap library.

The program can be terminated at any given moment with `Ctrl + C` sequence.

Scanning should be done and return results as fast as possible. During development and testing, try scanning only the computers you own or manage.

### Execution
```
./ipk-l2l3-scan [-i interface | --interface interface] {-w timeout} [-s ipv4-subnet | -s ipv6-subnet | --subnet ipv4-subnet | --subnet ipv6-subnet]
```

where:

* `-i eth0` (just one interface to scan through) or `--interface`. If this parameter is not specified (and any other parameters as well), or if only `-i/--interface` is specified without a value (and any other parameters are unspecified), a list of active interfaces is printed (additional information beyond the interface list is welcome but not required).
* `-w 3000` or `--wait 3000`, is the timeout in milliseconds to wait for a response for a single port scan. This parameter is optional, in its absence the value 5000 (i.e., five seconds) is used.
* `-s 192.168.1.0/24` or `--subnet fd00:cafe:0000:face::0/120` specifies which segments to scan using IPv4 or IPv6. There can be multiple segments to be scanned (i.e., the `-s/--subnet` argument can be repeated when the program is called).
* The application must be able to infer the correct network address and the resulting number of hosts to be scanned from the user input of the `-s` or `--subnet` argument.
* The application does not have to deal with the "bloat" of the `-s` or `--subnet` argument input with respect to the number of hosts being scanned (e.g., too short a netmask or prefix length) or the location of the segment being scanned (i.e.,attempting to ARP scan a network to which the computer is not directly connected).
* All arguments can be in any order.

### Execution Examples
```
./ipk-l2l3-scan -i eth0 -w 1000 -s 192.168.0.0/25 -s 192.168.128.0/29
./ipk-l2l3-scan --interface eth0 --wait 1000 --subnet fd00:cafe:0000:face::0/120
```

### Functionality Illustration
```
./ipk-l2l3-scan -i eth0 -w 1000 -s 192.168.0.5/25 -s 192.168.128.0/29 -s fd00:cafe:0000:face::1/126

Scanning ranges:
192.168.0.0/25 (126 hosts)
192.168.0.128/29 (6 hosts)
fd00:cafe:0000:face::0/127 (4 hosts)

192.168.0.1 arp OK (00-50-56-f1-c7-1b), icmpv4 OK
192.168.0.2 arp OK (00-22-14-ec-46-bb), icmpv4 FAIL
192.168.0.3 arp FAIL, icmpv4 FAIL
...
fd00:cafe:0000:face::0 ndp OK (00-50-56-f1-c7-1b), icmpv6 OK
fd00:cafe:0000:face::1 ndp FAIL, icmpv6 FAIL
fd00:cafe:0000:face::2 ndp OK (a8-5e-45-af-7c-60), icmpv6 FAIL
```

Illustrated command line output can be customised to provide relevant information in a more structured way.

## Bibliography
* RFC 792 - Internet Control Message Protocol
* RFC 4443 - Internet Control Message Protocol (ICMPv6) for the Internet Protocol Version 6 (IPv6) Specification
* RFC 4861 - Neighbor Discovery for IP version 6 (IPv6)
* RFC 826 - An Ethernet Address Resolution Protocol
* Host discovery - https://nmap.org/book/host-discovery-techniques.html  