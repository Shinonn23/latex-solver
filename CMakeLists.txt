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
add_subdirectory(src/tests)

# Add custom target to run tests automatically after build
# Use EXCLUDE_FROM_ALL to avoid dependency issues, then manually add dependencies
add_custom_command(
    TARGET latex-solver POST_BUILD
    COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure -C $<CONFIG>
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    COMMENT "Running tests after build..."
)
