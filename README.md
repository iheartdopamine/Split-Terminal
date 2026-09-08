# SPLIT-Core // Terminal Processing Protocol

An interactive, low-level binary data processing terminal simulation written in modern **C++20**, inspired by retro/dystopian data integrity interfaces (*S.P.L.I.T.*).

![C++20](https://img.shields.io/badge/C%2B%2B-20-blue.svg)
![Build](https://img.shields.io/badge/CMake-3.16%2B-green.svg)
![UI](https://img.shields.io/badge/UI-FTXUI-orange.svg)

## Overview
`SPLIT-Core` provides a real-time terminal user interface (TUI) that simulates raw binary buffer manipulation, sector corruption injection, and data stream splitting/purging.

## Features
- **Real-time TUI Layout:** Built with `FTXUI` featuring retro CRT aesthetics, custom colors, and dynamic panel updates.
- **Hexadecimal Memory Inspector:** Live rendering of a 64-byte raw buffer with hexadecimal formatting.
- **Data Integrity & Stream Splitting:** Low-level operations to inject and purge corrupted sector bytes (`0xFF`).
- **Event-Driven Terminal Loop:** Key navigation (`UP`/`DOWN` arrows, `ENTER`) handling dynamic system states.

## Architecture
```text
split-terminal/
├── include/
│   └── core/
│       ├── BufferManager.hpp  # Low-level memory & stream manipulation
│       └── System.hpp         # TUI event loop & layout orchestration
├── src/
│   ├── main.cpp
│   └── core/
│       ├── BufferManager.cpp
│       └── System.cpp
└── CMakeLists.txt             # C++20 CMake build definition
```

## Build & Run

### Prerequisites
- **C++20 Compiler** (MSVC 2019+, GCC 10+, or Clang 11+)
- **CMake** (3.16 or higher)

### Build Steps
```powershell
# Configure CMake build directory
cmake -B build

# Build project
cmake --build build

# Run executable
.\build\Debug\split_terminal.exe
