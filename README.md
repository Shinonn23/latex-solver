# LaTeX Solver

A C++ project for solving and manipulating LaTeX mathematical expressions.

## Project Structure

```
src/
├── main.cpp              # Entry point
├── cli/                  # Command-line interface
├── core/                 # Core solving logic
│   ├── ast/              # Abstract syntax tree definitions
│   ├── common/           # Common utilities
│   ├── rewrite/          # Expression rewriting rules
│   └── solver/           # Mathematical solvers
└── latex/                # LaTeX parsing and emission
    ├── lexer/            # LaTeX tokenization
    ├── parser/           # LaTeX parsing
    ├── lowering/         # AST conversion
    └── emitter/          # AST to LaTeX conversion
```

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

### VS Code Integration

- **Build Task:** Ctrl+Shift+B or Terminal → Run Build Task
- **Run:** Use the "CMake: Run" task
- **Debug:** F5 to start debugging with GDB

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
