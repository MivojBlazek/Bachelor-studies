CXX = g++
CXXFLAGS = -g -O2
LDFLAGS = -lsimlib -lm
BIN = model
SRC = main.cpp

all: compile

compile: $(SRC)
	$(CXX) $(CXXFLAGS) -o $(BIN) $(SRC) $(LDFLAGS)

run:
	./$(BIN)

clean:
	rm -f $(BIN)
