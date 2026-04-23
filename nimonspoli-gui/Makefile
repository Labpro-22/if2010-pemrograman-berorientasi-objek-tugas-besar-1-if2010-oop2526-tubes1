CXX      := g++
CXXFLAGS := -Wall -std=c++17 -I src -I lib/raylib/include
LDFLAGS  := -L lib/raylib/lib -lraylib

SRC_DIR := src
OBJ_DIR := build
BIN_DIR := bin
TARGET  := $(BIN_DIR)/nimonspoli

ifeq ($(OS),Windows_NT)
    LDFLAGS += -lglfw3 -lopengl32 -lgdi32 -lwinmm -luser32 -lshell32 -static-libgcc -static-libstdc++
else
    LDFLAGS += -lGL -lm -lpthread -ldl -lrt -lX11
endif

# Recursive source finding using wildcard for Windows compatibility
SRCS := $(wildcard $(SRC_DIR)/*.cpp) $(wildcard $(SRC_DIR)/**/*.cpp) $(wildcard $(SRC_DIR)/**/**/*.cpp) $(wildcard $(SRC_DIR)/**/**/**/*.cpp)
OBJS := $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRCS))

all: dirs $(TARGET)

dirs:
	@if not exist "$(OBJ_DIR)" mkdir "$(OBJ_DIR)"
	@if not exist "$(BIN_DIR)" mkdir "$(BIN_DIR)"

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)
	@echo "Build OK -> $(TARGET)"

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@if not exist "$(subst /,\,$(dir $@))" mkdir "$(subst /,\,$(dir $@))"
	$(CXX) $(CXXFLAGS) -c $< -o $@

run: all
	./$(TARGET)

clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

rebuild: clean all

.PHONY: all dirs run clean rebuild
