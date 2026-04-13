[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-22041afd0340ce965d47ae6ef1cefeee28c7c493a6346c4f15d667ab976d596c.svg)](https://classroom.github.com/a/b842RB8g)

# Tugas Besar 1 IF2010 Pemrograman Berorientasi Objek

Project Structure ini hanyalah referensi, Anda dapat menyesuaikannya dengan kebutuhan tim Anda

## How to Build

### Linux / WSL

1. Install the following dependencies:
  * C++17 compiler
    ```sh
    sudo apt install gcc 
    # or
    sudo apt install clang
    ```
  * CMake .16+
    ```sh
    sudo apt install cmake
    ```
  * SFML 2.5+
    ```
    sudo get install libsfml-dev
    ```
2. Build using CMake:
  ```sh
  cmake -S . -B build
  cmake --build build
  ```