CXX=g++
CXXFLAGS=-Wall -Wextra -Werror -pedantic -std=c++17
CXXFLAGSDEBUG=-Wall -Wextra -pedantic -std=c++17 -g

SRC=main.cpp DnsMonitor.cpp PacketCapturer.cpp Packet.cpp DnsHeader.cpp
PRG=dns-monitor

debug: $(SRC)
	$(CXX) $(CXXFLAGSDEBUG) -o $(PRG) $^ -lpcap

release: $(SRC)
	$(CXX) $(CXXFLAGS) -o $(PRG) $^ -lpcap

clean:
	$(RM) $(PRG)

.PHONY: debug release clean
