cmake_minimum_required(VERSION 3.10)
project(latex-solver VERSION 1.0.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Compiler-specific flags
if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -g3")
endif()

# Output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Source files - organize by components
set(CLI_SOURCES
    src/cli/derive.cpp
    src/cli/integrate.cpp
    src/cli/repl.cpp
    src/cli/simplify.cpp
    src/cli/solve.cpp
)

set(CORE_SOURCES
    # AST modules would go here
    # Common modules would go here
    # Rewrite modules would go here
    # Solver modules would go here
)

set(LATEX_SOURCES
    # LaTeX modules would go here
)

# Main executable
add_executable(latex-solver
    src/main.cpp
    ${CLI_SOURCES}
    ${CORE_SOURCES}
    ${LATEX_SOURCES}
)

# Include directories
target_include_directories(latex-solver PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)

# Optional: Add subdirectories for better organization
# add_subdirectory(src/cli)
# add_subdirectory(src/core)
# add_subdirectory(src/latex)

# Enable testing
enable_testing()

# Optional: Add tests
# add_subdirectory(src/tests)
