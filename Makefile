.PHONY: all build configure run clean

BUILD_DIR := build
GAME_BIN := bin/game

all: build

configure:
	cmake -S . -B $(BUILD_DIR)

build: configure
	cmake --build $(BUILD_DIR)

run: build
	./$(GAME_BIN)

clean:
	rm -rf $(BUILD_DIR) $(GAME_BIN)
