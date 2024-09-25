CXX=g++
CXXFLAGS=-Wall -Wextra -Werror -pedantic -std=c++17
CXXFLAGSDEBUG=-Wall -Wextra -pedantic -std=c++17 -g

SRC=main.cpp DnsMonitor.cpp PacketCapturer.cpp Packet.cpp
PRG=dns-monitor

debug: $(SRC)
	$(CXX) $(CXXFLAGSDEBUG) -o $(PRG) $^

release: $(SRC)
	$(CXX) $(CXXFLAGS) -o $(PRG) $^

clean:
	$(RM) $(PRG)

.PHONY: debug release clean
