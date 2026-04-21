[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-22041afd0340ce965d47ae6ef1cefeee28c7c493a6346c4f15d667ab976d596c.svg)](https://classroom.github.com/a/b842RB8g)

# Tugas Besar 1 IF2010 Pemrograman Berorientasi Objek

## 🎮 Monopoly Game - Object Oriented Programming Project

Project Structure ini hanyalah referensi, Anda dapat menyesuaikannya dengan kebutuhan tim Anda

---

## 🚀 Quick Start

### Prerequisites
- **C++ Compiler** (GCC, Clang, or MSVC)
- **CMake** 3.20+
- **raylib** (for GUI)

### Platform-Specific Installation

**macOS:**
```bash
brew install raylib cmake
```

**Linux (Ubuntu/Debian):**
```bash
sudo apt-get install libraylib-dev cmake build-essential pkg-config
```

**Windows:**
```cmd
# Using vcpkg
git clone https://github.com/Microsoft/vcpkg.git
cd vcpkg
.\bootstrap-vcpkg.bat
.\vcpkg install raylib:x64-windows
```

### Build & Run

**macOS/Linux:**
```bash
chmod +x build.sh
./build.sh run
```

**Windows:**
```cmd
build.bat run
```

**Manual CMake:**
```bash
mkdir -p build && cd build
cmake ..
cmake --build . --config Release
cd .. && ./bin/MonopolyGame
```

---

## 📋 Project Structure

```
.
├── include/
│   ├── core/              # Game logic headers
│   ├── gui/               # GUI/Graphics headers
│   ├── models/            # Data model headers
│   ├── utils/             # Utility headers
│   └── views/             # View headers
├── src/
│   ├── core/              # Game logic implementation
│   ├── gui/               # GUI implementation
│   ├── models/            # Data model implementation
│   ├── utils/             # Utilities implementation
│   ├── views/             # Views implementation
│   └── main.cpp           # Entry point
├── config/                # Configuration files
├── data/                  # Game data
├── bin/                   # Compiled executable
├── build/                 # Build artifacts (cmake)
├── CMakeLists.txt         # CMake configuration
├── build.sh               # Build helper (macOS/Linux)
├── build.bat              # Build helper (Windows)
├── PLATFORM_GUIDE.md      # Platform-specific instructions
├── CMAKE_SETUP_COMPLETE.md # Detailed CMake setup
└── README.md              # This file
```

---

## 🛠️ Build System

### CMake
Recommended build system with full cross-platform support:
- Windows (Visual Studio, MinGW)
- macOS (Clang)
- Linux (GCC, Clang)

### Makefile (Legacy)
For simple builds without external dependencies:
```bash
make run
make clean
```

### Helper Scripts
**macOS/Linux:**
```bash
./build.sh build              # Build
./build.sh run                # Build and run
./build.sh debug              # Debug build
./build.sh clean              # Clean build files
./build.sh rebuild            # Full rebuild
./build.sh help               # Show help
```

**Windows:**
```cmd
build.bat build               # Build
build.bat run                 # Build and run
build.bat debug               # Debug build
build.bat clean               # Clean build files
build.bat rebuild             # Full rebuild
build.bat help                # Show help
```

---

## 📖 Documentation

- **[PLATFORM_GUIDE.md](PLATFORM_GUIDE.md)** - Platform-specific setup instructions
- **[CMAKE_SETUP_COMPLETE.md](CMAKE_SETUP_COMPLETE.md)** - Detailed CMake configuration guide
- **[CMAKE_SETUP.md](CMAKE_SETUP.md)** - Quick CMake reference

---

## 🏗️ Architecture

### Core Module
- **GameManager** - Main game controller (singleton)
- **Board** - Game board with tiles
- **Player** - Player representation
- **Tile** - Base tile class with subclasses:
  - Street
  - Railroad
  - Utility
  - Tax (PPH, PBM)
  - Special (Go, Prison)

### GUI Module
- **BoardRenderer** - Renders game board
- **PawnRenderer** - Renders player pieces
- **DiceRenderer** - Renders dice
- **UIManager** - Manages UI elements
- **IsoTransformer** - Isometric projection math

### Data Management
- **DataManager** - Loads game configuration
- **CardDeck** - Card management system

---

## 🎯 Features

- ✅ Core Monopoly game logic
- ✅ Player management
- ✅ Tile system with properties
- ✅ Card system (skills, chance, currency)
- ✅ GUI with raylib
- ✅ Cross-platform support (Windows, macOS, Linux)
- ✅ CMake build system
- ✅ Data-driven configuration

---

## 🐛 Troubleshooting

### CMake Issues
See [CMAKE_SETUP_COMPLETE.md](CMAKE_SETUP_COMPLETE.md) for detailed troubleshooting

### Platform-Specific Issues  
See [PLATFORM_GUIDE.md](PLATFORM_GUIDE.md) for platform-specific solutions

### Common Issues
- **"raylib not found"**: Install raylib using platform-specific instructions
- **"C++ compiler not found"**: Install build tools for your platform
- **"CMake not found"**: Install CMake from cmake.org or package manager

---

## 📝 Build Configuration Files

- **CMakeLists.txt** - Main CMake configuration
- **makefile** - Legacy Makefile (without raylib)
- **build.sh** - macOS/Linux helper script
- **build.bat** - Windows helper script

---

## 🔄 Development Workflow

1. **Code Changes**
   ```bash
   # Make changes to src/ or include/
   git add .
   git commit -m "Your changes"
   ```

2. **Build**
   ```bash
   ./build.sh run      # macOS/Linux
   build.bat run       # Windows
   ```

3. **Test**
   ```bash
   # Run and test the application
   ```

4. **Clean Build (if needed)**
   ```bash
   ./build.sh rebuild  # macOS/Linux
   build.bat rebuild   # Windows
   ```

---

## 📊 Supported Platforms

| Platform | Compiler | Status | Tested |
|----------|----------|--------|--------|
| macOS 10.14+ | Clang | ✅ Supported | ✅ Yes |
| Ubuntu 20.04+ | GCC | ✅ Supported | ✅ Yes |
| Fedora 35+ | GCC | ✅ Supported | ⚠️ Partial |
| Windows 10/11 | MSVC | ✅ Supported | ✅ Yes |
| Arch Linux | GCC | ✅ Supported | ⚠️ Partial |

---

## 📞 Support

For issues or questions:
1. Check relevant documentation file
2. Review troubleshooting section in PLATFORM_GUIDE.md
3. Check CMake error messages

---

**Last Updated:** April 22, 2026