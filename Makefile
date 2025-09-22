CC=gcc
CXX=g++
CFLAGS=-Wall -Wextra -std=c11
CXXFLAGS=-Wall -Wextra -Wno-unused-parameter -std=c++17
SRC_C=$(wildcard src/*.c)
SRC_CPP=$(wildcard src/*.cpp)
SRC=$(SRC_C) $(SRC_CPP)
OBJ_C=$(SRC_C:.c=.o)
OBJ_CPP=$(SRC_CPP:.cpp=.o)
OBJ=$(OBJ_C) $(OBJ_CPP)
BIN=app

.PHONY: all run test clean

all: $(BIN)

$(BIN): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

run: $(BIN)
	./$(BIN)

test: $(BIN) tests/test_basic.sh
	bash tests/test_basic.sh

