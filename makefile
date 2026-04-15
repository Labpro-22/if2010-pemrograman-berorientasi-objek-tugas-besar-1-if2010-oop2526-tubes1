# Makefile for Nimonspoli

# Compiler settings
CXX := g++
CXXFLAGS := -Wall -Wextra -std=c++17 -I include

# Directories
SRC_DIR := src
OBJ_DIR := build
BIN_DIR := bin
DATA_DIR := data
CONFIG_DIR := config

# Target
TARGET := $(BIN_DIR)/game

# Recursive wildcard helper
rwildcard = $(wildcard $(1)/*.$(2)) $(foreach d,$(wildcard $(1)/*),$(call rwildcard,$(d),$(2)))

SRCS := $(call rwildcard,$(SRC_DIR),cpp)
OBJS := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRCS))

all: full

full: directories $(TARGET)

directories:
ifeq ($(OS),Windows_NT)
	@if not exist $(OBJ_DIR) mkdir $(OBJ_DIR)
	@if not exist $(BIN_DIR) mkdir $(BIN_DIR)
	@if not exist $(DATA_DIR) mkdir $(DATA_DIR)
	@if not exist $(CONFIG_DIR) mkdir $(CONFIG_DIR)
else
	@mkdir -p $(OBJ_DIR) $(BIN_DIR) $(DATA_DIR) $(CONFIG_DIR)
endif

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@
	@echo "Build successful: $(TARGET)"

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
ifeq ($(OS),Windows_NT)
	@powershell -NoProfile -Command "New-Item -ItemType Directory -Force -Path '$(subst /,\,$(dir $@))' | Out-Null"
else
	@mkdir -p $(dir $@)
endif
	$(CXX) $(CXXFLAGS) -c $< -o $@

run: full
	./$(TARGET)

clean:
ifeq ($(OS),Windows_NT)
	@if exist $(OBJ_DIR) rmdir /s /q $(OBJ_DIR)
	@if exist $(BIN_DIR) rmdir /s /q $(BIN_DIR)
else
	rm -rf $(OBJ_DIR) $(BIN_DIR)
endif
	@echo "Cleaned $(OBJ_DIR) and $(BIN_DIR)"

rebuild: clean full

.PHONY: all full run clean rebuild directories
