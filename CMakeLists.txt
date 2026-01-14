cmake_minimum_required(VERSION 3.10)
project(latex-solver VERSION 1.0.0 LANGUAGES CXX)

# =====================
# C++ standard
# =====================
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# =====================
# Compiler flags
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
# Sources
# =====================
set(CORE_SOURCES)
set(LATEX_SOURCES)

# =====================
# Main executable
# =====================
add_executable(latex-solver
    src/main.cpp
    ${CORE_SOURCES}
    ${LATEX_SOURCES}
)

target_include_directories(latex-solver
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# =====================
# Testing
# =====================
enable_testing()
add_subdirectory(src/tests)

# =====================
# Custom target to run tests
# =====================
add_custom_target(run-tests
    COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    COMMENT "Running tests..."
)
