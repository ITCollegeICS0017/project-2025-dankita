# Compiler settings
CC=gcc
CXX=g++
CFLAGS=-Wall -Wextra -std=c11
CXXFLAGS=-Wall -Wextra -Wno-unused-parameter -std=c++20
CPPFLAGS=-Isrc

# Collect sources from src and subfolders
SRC_DIRS=src src/config src/employees src/entities src/implementations src/managers src/orders src/types

SRC_C=$(foreach d,$(SRC_DIRS),$(wildcard $(d)/*.c))
SRC_CPP=$(foreach d,$(SRC_DIRS),$(wildcard $(d)/*.cpp))

OBJ_C=$(SRC_C:.c=.o)
OBJ_CPP=$(SRC_CPP:.cpp=.o)
OBJ=$(OBJ_C) $(OBJ_CPP)

BIN=app

.PHONY: all run test clean rebuild help

all: $(BIN)
	@echo "Build complete! Use 'make run' to execute."

$(BIN): $(OBJ)
	@echo "Linking $(BIN)..."
	@$(CXX) $(CXXFLAGS) $(CPPFLAGS) -o $@ $^

%.o: %.c
	@echo "Compiling $<..."
	@$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

%.o: %.cpp
	@echo "Compiling $<..."
	@$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $< -o $@

run: $(BIN)
	@./$(BIN)

test: $(BIN) tests/test_basic.sh
	@bash tests/test_basic.sh

clean:
	@rm -f $(OBJ) $(BIN) src/**/*.d src/*.d
	@echo "Clean complete"

rebuild: clean all

help:
	@echo "Available targets:"
	@echo "  make          - Build the project"
	@echo "  make -j4      - Build with 4 parallel jobs (faster!)"
	@echo "  make -jN      - Build with N parallel jobs"
	@echo "  make run      - Build and run the application"
	@echo "  make test     - Build and run tests"
	@echo "  make clean    - Remove all build artifacts"
	@echo "  make rebuild  - Clean and build from scratch"
	@echo "  make help     - Show this help message"

