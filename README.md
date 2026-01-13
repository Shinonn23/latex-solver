# LaTeX Solver

A C++ project for solving and manipulating LaTeX mathematical expressions.

## Build Instructions

### Prerequisites
- CMake 3.10 or higher
- G++ (C++17 compatible)
- Windows: MSYS2 with ucrt64 toolchain

### Build Steps

1. **Configure with CMake:**
   ```bash
   cmake -B build -S .
   ```

2. **Build the project:**
   ```bash
   cmake --build build
   ```

3. **Run the executable:**
   ```bash
   ./build/bin/latex-solver
   ```

### Compiler Flags
- `-Wall -Wextra`: Enable warnings
- `-g3`: Include debug symbols
- `-std=c++17`: Use C++17 standard

## Quick Commands

| Command | Purpose |
|---------|---------|
| `cmake -B build -S .` | Configure project |
| `cmake --build build` | Build project |
| `cmake --build build --target clean` | Clean build files |
| `./build/bin/latex-solver` | Run executable |

## Development

The project uses:
- **CMake** for build management
- **C++17** standard
- **Header-based organization** for core modules
