cmake_minimum_required(VERSION 3.10)
project(latex-solver VERSION 1.0.0 LANGUAGES CXX)

# =====================
# C++ standard
# =====================
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# =====================
# Compiler warnings
# =====================
if(MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra -g3)
endif()

# =====================
# Output directories
# =====================
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# =====================
# Core library
# =====================
add_library(latex_core
    src/core/eval/evaluator.cpp
    src/core/parser/parser.cpp
)

target_include_directories(latex_core
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# =====================
# Main executable
# =====================
add_executable(latex-solver
    src/main.cpp
)

target_link_libraries(latex-solver
    PRIVATE latex_core
)

# =====================
# Testing
# =====================
enable_testing()
add_subdirectory(src/tests)
