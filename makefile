CXX := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -pedantic -Iinclude
TARGET := bin/nimonspoli
SRC := $(shell find src -name '*.cpp')
OBJ := $(patsubst src/%.cpp,obj/%.o,$(SRC))

.PHONY: all clean dirs

all: $(TARGET)

$(TARGET): $(OBJ) | dirs
	$(CXX) $(CXXFLAGS) -o $@ $(OBJ)

obj/%.o: src/%.cpp | dirs
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

dirs:
	@mkdir -p bin obj

clean:
	rm -rf bin obj
