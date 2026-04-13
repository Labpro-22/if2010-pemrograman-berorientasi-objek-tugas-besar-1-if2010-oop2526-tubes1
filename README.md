[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-22041afd0340ce965d47ae6ef1cefeee28c7c493a6346c4f15d667ab976d596c.svg)](https://classroom.github.com/a/b842RB8g)

# Tugas Besar 1 IF2010 Pemrograman Berorientasi Objek

Project Structure ini hanyalah referensi, Anda dapat menyesuaikannya dengan kebutuhan tim Anda

## Project Structure

```
.
├── README.md
├── CMakeLists.txt
├── assets/                     # Aset gambar/avatar
├── build/                      # Build artifact and executable
├── config/                     # Configuration files
├── data/                       # Save/load files
├── include/
│   ├── core/                   # Domain entities
│   │   └── Tiles.hpp
│   ├── data/                   # Data Access Layer 
│   │   └── ConfigReader.hpp 
│   ├── logic/                  # Game logic
│   │   └── Game.hpp
│   └── ui/                     # GUI with SFML
│       ├── GameWindow.hpp
│       └── Widgets.hpp
└── src/
    ├── core/
    │   └── Tiles.cpp
    ├── data/
    │   └── ConfigReader.cpp
    ├── logic/
    │   └── Game.cpp
    ├── ui/
    │   ├── GameWindow.cpp
    │   └── Widgets.cpp
    └── main.cpp                # Main Executables

```

## How to Build

### Linux / WSL

1. Install the following dependencies:
  * C++17 compiler
    ```sh
    sudo apt install gcc 
    # or
    sudo apt install clang
    ```
  * CMake 3.16+
    ```sh
    sudo apt install cmake
    ```
  * SFML 2.5+
    ```
    sudo apt install libsfml-dev
    ```
2. Build using CMake:
  ```sh
  cmake -S . -B build
  cmake --build build
  ```