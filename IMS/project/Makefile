CXX = g++
CXXFLAGS = -g -O2
LDFLAGS = -lsimlib -lm
BIN = model
SRC = main.cpp

all: compile

compile: $(SRC)
	$(CXX) $(CXXFLAGS) -o $(BIN) $(SRC) $(LDFLAGS)

clean:
	rm -f $(BIN)
