#
# 
# @file: Makefile
# 
# @author: Michal Blažek <xblaze38>
# 
#

CXX=g++
CXXFLAGS=-Wall -Wextra -Werror -pedantic -std=c++17
CXXFLAGSDEBUG=-Wall -Wextra -pedantic -std=c++17 -g

SRC=main.cpp DnsMonitor.cpp PacketCapturer.cpp Packet.cpp DnsHeader.cpp
PRG=dns-monitor

release:
	$(CXX) $(CXXFLAGS) -o $(PRG) $(SRC) -lpcap

debug:
	$(CXX) $(CXXFLAGSDEBUG) -o $(PRG) $(SRC) -lpcap

clean:
	$(RM) $(PRG)

pack:
	tar -cvf xblaze38.tar DnsHeader.cpp DnsHeader.hpp DnsMonitor.cpp DnsMonitor.hpp main.cpp Makefile manual.pdf Packet.cpp Packet.hpp PacketCapturer.cpp PacketCapturer.hpp README

.PHONY: debug release clean pack
