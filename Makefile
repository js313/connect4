CXX := g++
CXXFLAGS := -std=c++20 -Wall -Wextra -Wpedantic -I./src

TARGET := bin/main

SRCS := $(wildcard src/*.cpp)
OBJS := $(SRCS:src/%.cpp=bin/%.o)

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@

bin/%.o: src/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f bin/*.o $(TARGET)

run:
	bin/main